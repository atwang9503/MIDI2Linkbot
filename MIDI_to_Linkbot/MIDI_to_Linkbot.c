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
#define DEFAULT_TEMPO 500000

unsigned long int tempo = DEFAULT_TEMPO;
typedef struct LBFile
{
	bool editted; // false by default, set to true when parser touches this file
	char name[80]; // holds the filename of the file
	FILE* fileptr; // points to the file that the object writes to
	unsigned long int time;
	bool noteOn; // NoteOn has been written to file with no termination yet
	int channel; // the note's channel as specified by the MIDI file.
	int note; // number denotes the note being played.
} LBFile;

char* IntToString(int i)
{
	char string[80];
	IntToString_helper(string, i);
	return string;
}// convert multiple digit integer to string representation

void IntToString_helper(char* string, int i)
{
	if (i / 10 != 0)
		IntToString_helper(string, i/10);
	strcat(string, i % 10 + '0');
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
LBFile* ParseModeZero(FILE* midi, const unsigned int divisions, char* filename)
{
	LBFile files[80];
	for (int i = 0; i < 80; i++) 
	{
		files[i].editted = false;
		strcpy(files[i].name, filename);
		strcat(files[i].name, IntToString(i+1));
		strcat(files[i].name, ".chf");
		files[i].time = 0;
		files[i].fileptr = fopen(files[i].name, "w");
		files[i].noteOn = false;
		files[i].channel = -1;
		files[i].note = -1;
	} // initialize LBFile objects
	unsigned char buffer[80];
	unsigned long long int absoluteTime = 0;
	fread(buffer, 1, 4, midi);  // Read in track header
	buffer[4] = '\0';
	assert(strcmp(buffer, "MTrk") == 0);
	unsigned long long int length = readBytes(buffer, midi, 4); //Read length of track
	for (int i = 0; i < length;)
	{
		long int current_pos = ftell(midi);
		// make cases for each event
		unsigned long long int deltaTime = variableQuantity(midi);
		fread(buffer, 1, 1, midi);
		if (buffer[0] == 0xFF)
		{
			// MIDI Meta-event: only care about set tempo, and absolute time has to be 0.
		}
		if ((buffer[0] >> 4) == 0x8)
		{
			//noteOn msg
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
const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
} // helper function for determining file compatibility credit: https://stackoverflow.com/questions/5309471/getting-file-extension-in-c

int main(int argc, char** argv)
{
	if (argc != 2 || argv[1] == NULL || strcmp(get_filename_ext(argv[1]), "mid") != 0) 
	{
		printf("Incorrect file format. Usage: midi2ll.exe [MIDI File]\n");
		exit(EXIT_FAILURE);	
	}
	
	char* filename = strtok(argv[1], ".");
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
	if (mode == 0b0000) 
	{
		printf("This MIDI file is format 0.\n");
		ParseModeZero(midi, divisions, filename);
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
