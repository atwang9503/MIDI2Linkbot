// MIDI_to_Linkbot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "MIDI_to_Linkbot.h"
#define DEFAULT_TEMPO	500000
#define MAX_BUFFER_SIZE	80
#define LBARRAY_SIZE	80
unsigned long int tempo = DEFAULT_TEMPO;

const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
} // helper function for determining file compatibility credit: https://stackoverflow.com/questions/5309471/getting-file-extension-in-c

char* IntToString(char* string, int number)
{
	int places = 0;
	IntToString_helper(string, number, &places);
	string[places] = '\0';
	return string;
}// convert multiple digit integer to string representation

void IntToString_helper(char* string, int i, int* places)
{
	(*places)++;
	if (i / 10 != 0)
		IntToString_helper(string, i / 10, places);
	unsigned char number = i % 10 + '0';
	strcat(string, &number);
	return;
}

unsigned long int variableQuantity(FILE* fileptr)
{
	unsigned long int value;
	variableQuantity_helper(fileptr, &value);
	return value;
}// gives back a long int representing the variable quantity value

void variableQuantity_helper(FILE* fileptr, unsigned long int* value)
{
	unsigned char buffer;
	fread(&buffer, 1, 1, fileptr);
	unsigned char val = buffer & 0b01111111;
	*value = *value | val;
	if (buffer >> 7 != 0) // MSB is 1 = more bytes to process
	{
		*value = *value << 7;
		variableQuantity_helper(fileptr, value);
	}
	return;
}

unsigned long int readBytes(unsigned char* buffer, FILE* fileptr, size_t num)
{
	unsigned long int value;
	size_t readed = fread(buffer, 1, num, fileptr);
	assert(readed == num);
	for (unsigned int i = 0; i < num; i++)
	{	
		if (i > 0)
			value = value << 8;
		value = value | buffer[i];
	}
	return value;
}//assumes that unsigned char array has sufficient space, reads in <num> amount of bytes from stream and outputs in a combined binary format stored in unsigned long int

int PlaceNote(LBFile** files, size_t startIndex, size_t filesSize, unsigned long int absoluteTime, int noteNumber, int channelNumber, const unsigned int divisions)
{
	size_t i = startIndex;
	for (; i < filesSize; i++)
	{
		if (!files[i]->noteOn)
			break;
	}
	files[i]->editted = true;
	files[i]->noteOn = true;
	files[i]->note = noteNumber;
	files[i]->channel = channelNumber;
	unsigned long int delta = absoluteTime - files[i]->time;
	if (delta > 0)
	{
		double time = 1e6 / (delta * tempo / divisions);
		fprintf(files[i]->fileptr, "\t\t{0, %lf}, \n", time);
		files[i]->time = absoluteTime;
		return 2;
	}
	else
		return 1;
	return 0;
}// Starts a new note on a LB File. Returns 2 if a pause note was placed, 1 if no note was placed, 0 if error occurred

int EndNote(LBFile** files, size_t startIndex, size_t filesSize, unsigned long int absoluteTime, int noteNumber, int channelNumber, const unsigned int divisions)
{
	size_t i = startIndex;
	for (; i < filesSize && files[i]->editted; i++)
	{
		if (files[i]->noteOn && files[i]->note == noteNumber && files[i]->channel == channelNumber)
			break;
	}
	if (!files[i]->editted)
		return 0;
	files[i]->noteOn = false;
	files[i]->note = -1;
	files[i]->channel = -1;
	unsigned long int delta = absoluteTime - files[i]->time;
	if (delta > 0)
	{
		double time = 1e6 / (delta * tempo / divisions);
		fprintf(files[i]->fileptr, "\t\t{NOTE_%s, %lf}, \n", NOTES[noteNumber], time);
		files[i]->time = absoluteTime;
		return 2;
	}
	return 1;
}// finds the LBFile playing a note and ends it. returns 2 if a note was placed, 1 if no note was placed, 0 if error occurred

size_t ParseModeZero(LBFile** files, size_t startIndex, size_t filesSize, FILE* midi, const unsigned int divisions, char* filename)
{
	unsigned char buffer[MAX_BUFFER_SIZE]; // General purpose buffer for reading in data
	unsigned long int absoluteTime = 0; // Measures time elapsed in the track in ticks whose duration is defined by meta-events SetTempo and Track Header
	fread(buffer, 1, 4, midi);  // Read in track header
	buffer[4] = '\0';
	assert(strcmp(buffer, "MTrk") == 0);
	size_t length = fread(&length, 1, 4, midi); //Read length of track
	printf("MTrk Length: %zd\n", length);
	int percussionOn = -1;
	for (size_t i = 0; i < length;)
	{
		unsigned long int current_pos = ftell(midi); // Get the current seek position to increment i.
		// make cases for each event
		unsigned long int deltaTime = variableQuantity(midi);
		absoluteTime += deltaTime;
		fread(buffer, 1, 1, midi);
		if (buffer[0] == msgMetaEvent)
		{
			fread(buffer, 1, 1, midi);
			if (buffer[0] == metaEndSequence) //signals end of track.
			{
				fread(buffer, 1, 1, midi);
				unsigned long int change = current_pos - ftell(midi);
				if (buffer[0] == 0x00 && i + change == length - 1)
					break; //get out of for loop
			}	// MIDI Meta-event: End of track
			if (buffer[0] == metaSetTempo && absoluteTime == 0)
			{
				fread(buffer, 1, 1, midi); // skip the 0x03 byte
				assert(buffer[0] == 0x03);
				fread(buffer, 1, 3, midi);
				unsigned long int newTempo = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
				printf("Set Tempo: %lu\n", newTempo);
				tempo = newTempo;
			}	// MIDI Meta-event: set tempo only when absolute time is 0.
		}
		else if ((buffer[0] & 0b11110000) == msgNoteOn)
		{
			//noteOn msg
			int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
			fread(buffer, 1, 1, midi);
			int noteNumber = buffer[0];
			assert(noteNumber <= 127 && channel <= 16 && channel >= 0);
			fread(buffer, 1, 1, midi);
			int volume = buffer[0]; // don't care about volume
			assert(volume >= 0 && volume < 256);
			if (channel != 9 && percussionOn != channel)
				PlaceNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
		}
		else if ((buffer[0] >> 4 == 0x9))
		{
			//noteOff msg
			int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
			fread(buffer, 1, 1, midi);
			int noteNumber = buffer[0];
			assert(noteNumber <= 127 && channel <= 16 && channel >= 0);
			fread(buffer, 1, 1, midi);
			int volume = buffer[0]; // don't care about volume
			assert(volume >= 0 && volume < 256);
			if (channel != 9 && percussionOn != channel)
				EndNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
		}
		else if ((buffer[0] & 11110000) == msgSetProgram)
		{
			int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
			fread(buffer, 1, 1, midi);
			int programNum = buffer[0];
			assert(programNum >= 0 && programNum <= 127);
			if (programNum >= 112)
				percussionOn = channel;
			else
				percussionOn = -1;
			// Program Change
		}// also care if Program Change. Ignore notes being played by percussion on the channel specified.
		i += ftell(midi) - current_pos;
	}
	size_t i = startIndex;
	for (; i < filesSize; i++)
	{
		if (!files[i]->editted)
			break;
	}
	return i-startIndex;
} // returns the number of files written to in the course of parsing.
size_t ParseModeOne(FILE* midi, size_t filesSize, const unsigned int divisions)
{
	return 0;
} //runs ParseModeZero on each track then combines the files
int main(int argc, char** argv)
{
	if (argc != 2 || argv[1] == NULL || strcmp(get_filename_ext(argv[1]), "mid") != 0) 
	{
		printf("Incorrect file format. Usage: midi2ll.exe [MIDI File]\n");
		exit(EXIT_FAILURE);	
	}
	
	char filename[MAX_BUFFER_SIZE];
	strcpy(filename, argv[1]);
	strtok(filename, ".");
	FILE* midi = fopen(argv[1], "rb");
	if (!midi)
	{
		printf("Could not open MIDI File.\n");
		exit(EXIT_FAILURE);
	}
	printf("MIDI Song: %s\n", filename);
	char header[5] = "";
	fread(header, 1, 4, midi);
	assert(strcmp(header, "MThd") == 0);
	fseek(midi, 3, SEEK_CUR);
	char length;
	fread(&length, 1, 1, midi);
	assert((int)length == 6);
	char mode;
	fseek(midi, 1, SEEK_CUR);
	fread(&mode, 1, 1, midi);
	unsigned char ntrk[2];
	fread(ntrk, 1, 2, midi);
	int numtrack = ntrk[0] << 8 | ntrk[1];
	unsigned char div[2];
	fread(div, 1, 2, midi);
	const unsigned int divisions = (0xFF & div[0]) << 8 | 0xFF & div[1];
	printf("ntrk: %u, divisions: %u\n", numtrack, divisions);
	LBFile* files[LBARRAY_SIZE];
	for (unsigned int i = 0; i < LBARRAY_SIZE; i++)
	{
		files[i] = (LBFile*)malloc(sizeof(LBFile));
		files[i]->editted = false;
		strcpy(files[i]->name, filename);
		char number[10] = "";
		IntToString(number, i + 1);
		strcat(files[i]->name, number);
		strcat(files[i]->name, ".chf");
		files[i]->time = 0;
		files[i]->fileptr = fopen(files[i]->name, "w");
		files[i]->noteOn = false;
		files[i]->channel = -1;
		files[i]->note = -1;
	} // initialize LBFile objects. Unused objects will be deleted after termination of program.
	if (mode == 0b0000) 
	{
		printf("This MIDI file is format 0.\n");
		ParseModeZero(files, 0, LBARRAY_SIZE, midi, divisions, filename);
	} // enter single track mode
	else if (mode == 0b0001)
	{
		printf("This MIDI file is format 1.\n");
	} // enter simultaneous track mode
	else if (mode == 0b0010)
	{
		printf("This MIDI file uses sequential track mode, which is not yet supported.\n");
	} // sequential track mode
	fclose(midi);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
