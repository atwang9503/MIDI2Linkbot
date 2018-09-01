// MIDI_to_Linkbot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
	int divisions = (0xFF & div[0]) << 8 | 0xFF & div[1];
	printf("ntrk: %u, divisions: %u\n", numtrack, divisions);
	if (mode == 0b0000) 
	{
		printf("This MIDI file is format 0.\n");
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
