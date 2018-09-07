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

unsigned long long int variableQuantity(FILE* fileptr)
{
	unsigned long long int value;
	variableQuantity_helper(fileptr, &value);
	return value;
}// gives back a long long int representing the variable quantity value

void variableQuantity_helper(FILE* fileptr, unsigned long long int* value)
{
	unsigned char buffer;
	fread(&buffer, 1, 1, fileptr);
	unsigned char val = buffer & 0b01111111;
	*value = *value | val;
	if (buffer >> 7 != 0)
	{
		*value = *value << 7;
		variableQuantity_helper(fileptr, value);
	}
	return;
}

unsigned long long int readBytes(unsigned char* buffer, FILE* fileptr, unsigned int num)
{
	unsigned long long int value;
	int readed = fread(buffer, 1, num, fileptr);
	assert(readed == num);
	for (int i = 0; i < num; i++)
	{	
		if (i > 0)
			value = value << 8;
		value = value | buffer[i];
	}
	return value;
}/*assumes that unsigned char array has sufficient space, reads in <num> amount of bytes from stream 
and outputs in a combined binary format stored in unsigned long long int
*/

int PlaceNote(LBFile** files, unsigned int filesSize, unsigned long long int absoluteTime, unsigned int noteNumber, unsigned int channelNumber)
{
	size_t i = 0;
	for (; !files[i]->noteOn && i < filesSize; i++);
	files[i]->editted = true;
	files[i]->noteOn = true;
	files[i]->note = noteNumber;
	files[i]->channel = channelNumber;
	unsigned long int delta = absoluteTime - files[i]->time;
	if (delta > 0)
	{
		//TODO: Place a pause note for the amount of time difference between last note and current absoluteTime
	}
}// Starts a new note on a LB File.

int EndNote(LBFile** files, unsigned long long int absoluteTime, unsigned int noteNumber, unsigned int channelNumber)
{
	size_t i = 0;
	for (; files[i]->noteOn && files[i]->note == noteNumber && files[i]->channel == channelNumber; i++);
	files[i]->noteOn = false;
	files[i]->note = -1;
	files[i]->channel = -1;
	unsigned long int delta = absoluteTime - files[i]->time;
	if (delta > 0)
	{
		// TODO: Place the note that just ended into the array.
	}
}// finds the LBFile playing the particular note to be ended on the specific channel

LBFile** ParseModeZero(LBFile** files, FILE* midi, const unsigned int divisions, char* filename)
{

	for (int i = 0; i < LBARRAY_SIZE; i++) 
	{
		files[i] = (LBFile*) malloc(sizeof(LBFile));
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
	} // initialize LBFile objects
	unsigned char buffer[MAX_BUFFER_SIZE]; // General purpose buffer for reading in stuff
	unsigned long long int absoluteTime = 0;
	fread(buffer, 1, 4, midi);  // Read in track header
	buffer[4] = '\0';
	assert(strcmp(buffer, "MTrk") == 0);
	unsigned long long int length = fread(&length, 1, 4, midi); //Read length of track
	for (int i = 0; i < length;)
	{
		long int current_pos = ftell(midi);
		// make cases for each event
		unsigned long long int deltaTime = variableQuantity(midi);
		fread(buffer, 1, 1, midi);
		if (buffer[0] == 0xFF)
		{
			// MIDI Meta-event: set tempo only when absolute time is 0.
			// MIDI Meta-event: End of track
		}
		if ((buffer[0] >> 4) == 0x8)
		{
			//noteOn msg
			int channel = buffer[0] & 0b1111;
			fread(buffer, 1, 1, midi);
			unsigned int noteNumber = buffer[0];
			assert(noteNumber <= 127 && channel <= 16 && channel >= 0);

		}
		if ((buffer[0] >> 4 == 0x9))
		{
			//noteOff msg
		}
		if ((buffer[0] >> 4 == 0xC))
		{
			// Program Change
		}// also care if Program Change. Ignore notes being played by percussion on channel 10.
	}
	return files;
} // returns the array of LBFile Objects that point to the files.
LBFile* ParseModeOne(FILE* midi, const unsigned int divisions)
{
	return NULL;
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
	unsigned int divisions = (0xFF & div[0]) << 8 | 0xFF & div[1];
	printf("ntrk: %u, divisions: %u\n", numtrack, divisions);
	LBFile* files[LBARRAY_SIZE];
	if (mode == 0b0000) 
	{
		printf("This MIDI file is format 0.\n");
		//ParseModeZero(midi, divisions, filename);
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
