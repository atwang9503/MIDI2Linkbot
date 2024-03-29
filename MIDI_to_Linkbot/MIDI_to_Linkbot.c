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
#define LBARRAY_SIZE	256

unsigned long int tempo = DEFAULT_TEMPO;

char* removeSpaces(char *str)
{
	// To keep track of non-space character count
	int count = 0;

	// Traverse the given string. If current character
	// is not space, then place it at index 'count++'
	for (int i = 0; str[i]; i++)
		if (str[i] != ' ' && str[i] != '-' && str[i] != '_')
			str[count++] = str[i]; // here count is
								   // incremented
	str[count] = '\0';
	return str;
} //credit to: https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/

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
} // convert multiple digit integer to string representation

void IntToString_helper(char* string, int i, int* places)
{
	(*places)++;
	if (i / 10 != 0)
		IntToString_helper(string, i / 10, places);
	unsigned char number[2] = { i % 10 + '0', '\0' };
	strcat(string, number);
	return;
}

unsigned long int variableQuantity(FILE* fileptr)
{
	unsigned long int value = 0;
	variableQuantity_helper(fileptr, &value);
	return value;
}// gives back a long int representing the variable quantity value read from stream

void variableQuantity_helper(FILE* fileptr, unsigned long int* value)
{
	unsigned char buffer;
	fread(&buffer, 1, 1, fileptr);
	unsigned char val = buffer & 0b01111111;
	*value = *value | val;
	if (buffer >> 7 != 0) // MSB is 1 = more bytes to process
	{
		*value = (*value) << 7;
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
	for (; i <= filesSize; i++)
	{
		if (i == filesSize)
		{
			printf("ERROR: Could not place note due to insufficient filesSize.\n");
			return 0;
		}
		if (files[i]->time <= absoluteTime)
		{
			if (!files[i]->noteOn)
				break;
		}
	}
	files[i]->editted = true;
	files[i]->noteOn = true;
	files[i]->note = noteNumber;
	files[i]->channel = channelNumber;
	unsigned long int delta = absoluteTime - files[i]->time;
	if (delta > 0)
	{
		double time = 1e6 / (delta * tempo / divisions); // tempo states how many microseconds are in a beat(QN by default) delta gives the number of ticks that have passed. 
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
	for (; i <= filesSize && files[i]->editted; i++)
	{
		if (i == filesSize)
		{
			printf("ERROR: Could not place note due to insufficient filesSize.\n");
			return 0;
		}
		if (files[i]->time <= absoluteTime)
		{
			if (files[i]->noteOn && files[i]->note == noteNumber && files[i]->channel == channelNumber)
				break;
		}
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
		fprintf(files[i]->fileptr, "\t\t{%s, %lf}, \n", NOTES2[noteNumber], time);
		files[i]->time = absoluteTime;
		return 2;
	}
	return 1;
} // finds the LBFile playing a note and ends it. returns 2 if a note was placed, 1 if no note was placed, 0 if error occurred

long int SkipSystemCommon(FILE* midi)
{
	char buffer;
	fread(&buffer, 1, 1, midi);
	if (buffer == msgSysEx1) // Status byte 0xF0
	{
		long int count = 1;
		while (buffer != msgSysEx2) // Byte 0xF7
		{
			fread(&buffer, 1, 1, midi);
			count++;
		}
		return count;
	}
	else if (buffer == 0xF1 || buffer == 0xF3) // MIDI Time Code Quarter Frame or MIDI Song Request
	{
		fseek(midi, 1, SEEK_CUR);
		return 2;
	}
	else if (buffer == 0xF2) // Song Position Pointer
	{
		fseek(midi, 2, SEEK_CUR);
		return 3;
	}
	else if (buffer == 0xF6) // MIDI Tune Request
	{
		return 1;
	}
	else
	{
		fseek(midi, -1, SEEK_CUR);
		return 0;
	}
 } // Skips any System common messages and returns the number of bytes skipped

long int SkipSystemRealtime(FILE* midi)
{
	char buffer;
	fread(&buffer, 1, 1, midi);
	if (buffer == 0xF8 || 0xFA || 0xFB || 0xFC || 0xFE) // MIDI clock, start, continue, stop, active sense (reset is not stored in the MIDI file. Neither are these, probably)
		return 1;
	return 0;
} // Skips any System realtime messages and returns the number of bytes skipped

size_t ParseModeZero(LBFile** files, size_t startIndex, size_t filesSize, FILE* midi, const unsigned int divisions, FILE* debugPtr)
{
	unsigned char buffer[MAX_BUFFER_SIZE]; // General purpose buffer for reading in data
	unsigned long int absoluteTime = 0; // Measures time elapsed in the track in ticks whose duration is defined by meta-events SetTempo and Track Header
	fread(buffer, 1, 4, midi);  // Read in track header
	buffer[4] = '\0';
	fprintf(debugPtr, "%s", buffer);
	assert(strcmp(buffer, "MTrk") == 0);
	unsigned char array[4];
	fread(array, 1, 4, midi); //Read length of track
	size_t length = array[0] << 24 | array[1] << 16 | array[2] << 8 | array[3];
	fprintf(debugPtr, " Length: %zu\n", length);
	unsigned long int current_pos = ftell(midi); // Get the current seek position
	unsigned char runningStatus = 0xFF; // holds the code for the previous channel message (unless if the previous message was a SysEx or Meta-event)
	bool channelPercussion[16]; // Denotes if percussion is playing on that channel. if there is, place/end note is ignored
	for (size_t i = 0; i < 16; i++)
	{
		if (i != 9)
			channelPercussion[i] = false;
	}
	for (size_t i = 0; i < length; i += ftell(midi) - current_pos)
	{
		current_pos = ftell(midi);
		unsigned long int deltaTime = variableQuantity(midi); // Preceding each MIDI event is a variable quantity delta time measured in ticks
		fprintf(debugPtr, "delta: %u ", deltaTime);
		absoluteTime += deltaTime; // Keeps track of current time in track.
		fread(buffer, 1, 1, midi);
		if (SkipSystemCommon(midi) != 0) // Skips system common messages
		{
			fprintf(debugPtr, "Skipped System Common Message at byte %li", ftell(midi));
			runningStatus = 0xFF; // set running status to null when message is meta event or sys common
			continue;
		}
		if (buffer[0] == msgMetaEvent) // Byte code: 0xFF
		{
			fread(buffer, 1, 1, midi);
			if (buffer[0] == metaEndSequence) // Byte code: 0x2F signals end of track.
			{
				fprintf(debugPtr, "Meta: End track at byte %li\n", ftell(midi) - 2);
				fread(buffer, 1, 1, midi);
				unsigned long int change = ftell(midi) - current_pos;
				assert(i + change == length);
				if (buffer[0] == 0x00)
				{
					runningStatus = 0xFF; // set running status to null
					break; //get out of for loop
				}
			}	// MIDI Meta-event: End of track
			else if (buffer[0] == metaSetTempo && absoluteTime == 0) // Byte code: 0x51
			{
				fprintf(debugPtr, "Meta: Set Tempo at byte %li\n", ftell(midi) - 2);
				fread(buffer, 1, 1, midi); // skip the 0x03 byte
				assert(buffer[0] == 0x03);
				fread(buffer, 1, 3, midi); // 3 bytes denote the microseconds per beat
				unsigned long int newTempo = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
				fprintf(debugPtr, "Set Tempo: %lu\n", newTempo);
				tempo = newTempo;
			}	// MIDI Meta-event: set tempo only when absolute time is 0.
			else
			{
				unsigned long int beforeVarQ = ftell(midi);
				unsigned long int skip = variableQuantity(midi);
				fprintf(debugPtr, "Irrelevant Meta-Event at byte %li\tSkipping %lu bytes\n", beforeVarQ - 2, skip);
				fseek(midi, skip, SEEK_CUR);
			} // skip over the meta-event
		}
		else
		{
			if ((buffer[0] & 0x80) != 0) // bit masked with MSB = 1. If true, there is a status byte in the message
			{
				if (buffer[0] >= msgNoteOn && buffer[0] <= msgNoteOn + 0xF)
				{
					//noteOn msg
					fprintf(debugPtr, "\tVoice: non-RS NoteOn at byte %li ", ftell(midi) - 1);
					runningStatus = buffer[0];
					int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
					fread(buffer, 1, 1, midi);
					int noteNumber = buffer[0];
					assert(noteNumber <= 127 && channel < 16 && channel >= 0);
					fread(buffer, 1, 1, midi);
					int velocity = buffer[0]; // don't care about velocity
					assert(velocity >= 0 && velocity < 256);
					fprintf(debugPtr, "\tchannel: %d, note: %d, velocity: %d\n", channel, noteNumber, velocity);
					if (velocity > 0 && !channelPercussion[channel]) // check if percussion is playing on the channel
						PlaceNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
					else if (velocity == 0 && !channelPercussion[channel])
						EndNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions); // velocity 0 noteOn, which is just noteOff
				}
				else if (buffer[0] >= msgNoteOff && buffer[0] <= msgNoteOff + 0xF)
				{
					//noteOff msg
					fprintf(debugPtr, "\tVoice: non-RS NoteOff at byte %li ", ftell(midi) - 1);
					runningStatus = buffer[0];
					int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
					fread(buffer, 1, 1, midi);
					int noteNumber = buffer[0];
					assert(noteNumber <= 127 && channel < 16 && channel >= 0);
					fread(buffer, 1, 1, midi);
					int velocity = buffer[0]; // don't care about velocity
					assert(velocity >= 0 && velocity < 256);
					fprintf(debugPtr, "\tchannel: %d, note: %d, velocity: %d\n", channel, noteNumber, velocity);
					if (!channelPercussion[channel]) // check if percussion is playing on the channel
						EndNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
				}
				else if (buffer[0] >= msgSetProgram && buffer[0] <= msgSetProgram + 0xF)
				{
					fprintf(debugPtr, "\tVoice: non-RS Program Change at byte %li\n", ftell(midi) - 1);
					runningStatus = buffer[0];
					int channel = buffer[0] & 0b1111; // Get lower 4 bits which denotes the channel number
					fread(buffer, 1, 1, midi);
					int programNum = buffer[0];
					assert(programNum >= 0 && programNum <= 127);
					if (channel == 9)
						continue;
					if (programNum >= 112)
						channelPercussion[channel] = true;
					else
						channelPercussion[channel] = false;
				}// Program Change. Ignore notes being played by percussion on the channel specified.
				else if (buffer[0] >= msgKeyPressure && buffer[0] <= msgKeyPressure + 0xF)
				{
					fprintf(debugPtr, "\tVoice: non-RS Key Pressure at byte %li\n", ftell(midi) - 1);
					runningStatus = buffer[0];
					fseek(midi, 2, SEEK_CUR);
				}
				else if (buffer[0] >= msgSetParameter && buffer[0] <= msgSetParameter + 0xF)
				{
					fprintf(debugPtr, "\tVoice: non-RS Controller at byte %li\n", ftell(midi) - 1);
					runningStatus = buffer[0];
					fseek(midi, 2, SEEK_CUR);
				}
				else if (buffer[0] >= msgChangePressure && buffer[0] <= msgChangePressure + 0xF)
				{
					fprintf(debugPtr, "\tVoice: non-RS Change Pressure at byte %li\n", ftell(midi) - 1);
					runningStatus = buffer[0];
					fseek(midi, 1, SEEK_CUR);
				}
				else if (buffer[0] >= msgSetPitchWheel && buffer[0] <= msgSetPitchWheel + 0xF)
				{
					fprintf(debugPtr, "\tVoice: non-RS Pitch Modulation at byte %li\n", ftell(midi) - 1);
					runningStatus = buffer[0];
					fseek(midi, 2, SEEK_CUR);
				}
				else
				{
					fprintf(debugPtr, "FAILURE: Error parsing MIDI message. (Branch: MIDI voice msg (non-RS))\n");
					exit(EXIT_FAILURE);
				}
			}
			else if ((buffer[0] & 0x80) == 0) // no status byte, INITIATE RUNNING STATUS
			{ // in runningStatus mode, buffer[0] already holds the first byte value.
				if (runningStatus >= msgNoteOn && runningStatus <= msgNoteOn + 0xF)
				{
					//noteOn msg
					fprintf(debugPtr, "\tVoice: RS NoteOn at byte %li\n", ftell(midi));
					int channel = runningStatus & 0b1111; // Get lower 4 bits which denotes the channel number
					int noteNumber = buffer[0];
					assert(noteNumber <= 127 && channel < 16 && channel >= 0);
					fread(buffer, 1, 1, midi);
					int velocity = buffer[0]; // don't care about velocity, unless if it's 0
					assert(velocity >= 0 && velocity < 256);
					fprintf(debugPtr, "\tchannel: %d, note: %d, velocity: %d\n", channel, noteNumber, velocity);
					if (velocity > 0 && !channelPercussion[channel])
						PlaceNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
					else if (velocity == 0 && !channelPercussion[channel])
						EndNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions); // velocity 0 noteOn, which is just noteOff
				}
				else if (runningStatus >= msgNoteOff && runningStatus <= msgNoteOff + 0xF)
				{
					//noteOff msg
					fprintf(debugPtr, "\tVoice: RS NoteOff at byte %li\n", ftell(midi));
					int channel = runningStatus & 0b1111; // Get lower 4 bits which denotes the channel number
					int noteNumber = buffer[0];
					assert(noteNumber <= 127 && channel < 16 && channel >= 0);
					fread(buffer, 1, 1, midi);
					int velocity = buffer[0]; // don't care about velocity
					assert(velocity >= 0 && velocity < 256);
					fprintf(debugPtr, "\tchannel: %d, note: %d, velocity: %d\n", channel, noteNumber, velocity);
					if (!channelPercussion[channel])
						EndNote(files, startIndex, filesSize, absoluteTime, noteNumber, channel, divisions);
				}
				else if (runningStatus >= msgSetProgram && runningStatus <= msgSetProgram + 0xF)
				{
					fprintf(debugPtr, "\tVoice: RS Program Change at byte %li\n", ftell(midi));
					int channel = runningStatus & 0b1111; // Get lower 4 bits which denotes the channel number
					int programNum = buffer[0];
					assert(programNum >= 0 && programNum <= 127);
					if (channel == 9)
						continue;
					if (programNum >= 112)
						channelPercussion[channel] = true;
					else
						channelPercussion[channel] = false;
				}// Program Change. Ignore notes being played by percussion on the channel specified.
				else if (runningStatus >= msgKeyPressure && runningStatus <= msgKeyPressure + 0xF) // fseeks are reduced by 1 since buffer already held the first byte
				{
					fprintf(debugPtr, "\tVoice: RS Key Pressure at byte %li\n", ftell(midi));
					fseek(midi, 1, SEEK_CUR);
				}
				else if (runningStatus >= msgSetParameter && runningStatus <= msgSetParameter + 0xF)
				{
					fprintf(debugPtr, "\tVoice: RS Controller at byte %li\n", ftell(midi));
					fseek(midi, 1, SEEK_CUR);
				}
				else if (runningStatus >= msgChangePressure && runningStatus <= msgChangePressure + 0xF)
				{
					fprintf(debugPtr, "\tVoice: RS Change Pressure at byte %li\n", ftell(midi));
					fseek(midi, 0, SEEK_CUR);
				}
				else if (runningStatus >= msgSetPitchWheel && runningStatus <= msgSetPitchWheel + 0xF)
				{
					fprintf(debugPtr, "\tVoice: RS Pitch Modulation at byte %li\n", ftell(midi));
					fseek(midi, 1, SEEK_CUR);
				}
				else
				{
					fprintf(debugPtr, "FAILURE: Error parsing MIDI message. (Branch: MIDI voice msg (RunningStatus = 0x%x))\n", runningStatus);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				fprintf(debugPtr, "FAILURE: Error parsing MIDI message. (FALL_THROUGH)\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	size_t filesEditted = startIndex;
	for (; filesEditted < filesSize; filesEditted++)
	{
		if (!files[filesEditted]->editted)
			break;
	} // check how many files were written to.
	return filesEditted-startIndex;
} // returns the number of files written to in the course of parsing.
size_t ParseModeOne(LBFile** files, size_t filesSize, FILE* midi, const unsigned int numtrack, const unsigned int divisions, FILE* debugPtr)
{
	long int current_pos = ftell(midi);
	fseek(midi, 0, SEEK_END); // Get the ending position
	long int end_pos = ftell(midi);
	fseek(midi, current_pos, SEEK_SET); // Set marker back to original position
	size_t numFiles = 0;
	size_t i = 0;
	while (ftell(midi) < end_pos && i < numtrack)
	{
		numFiles = ParseModeZero(files, 0, LBARRAY_SIZE, midi, divisions, debugPtr);
		i++;
	}
	return numFiles;
} //runs ParseModeZero on each track, returns total number of files used.
int main(int argc, char** argv)
{
	if (argc != 2 || argv[1] == NULL || strcmp(get_filename_ext(argv[1]), "mid") != 0) 
	{
		printf("Incorrect file format. Usage: midi2ll.exe [MIDI File]\n");
		exit(EXIT_FAILURE);	
	}
	
	char filename[MAX_BUFFER_SIZE];
	strcpy(filename, argv[1]);
	removeSpaces(filename);
	strtok(filename, ".()[]!@#$%^&*");
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
	unsigned int numtrack = ntrk[0] << 8 | ntrk[1];
	unsigned char div[2];
	fread(div, 1, 2, midi);
	if ((div[0] & 0x8) != 0)
	{
		printf("This MIDI uses SMPTE format, which is not yet supported by the parser.\n");
		exit(EXIT_FAILURE);
	}
	char debug[80] = "";
	strcpy(debug, filename);
	strcat(debug, "Debug.txt");
	FILE* debugPtr = fopen(debug, "w");
	const unsigned int divisions = (0x7F & div[0]) << 8 | 0xFF & div[1];
	fprintf(debugPtr, "ntrk: %u, divisions: %u\n", numtrack, divisions);
	LBFile* files[LBARRAY_SIZE];
	for (size_t i = 0; i < LBARRAY_SIZE; i++)
	{
		files[i] = (LBFile*)malloc(sizeof(LBFile));
		files[i]->editted = false;
		strcpy(files[i]->name, filename);
		char number[10] = "";
		IntToString(number, (int)i + 1);
		strcat(files[i]->name, number);
		strcpy(files[i]->functionName, files[i]->name);
		strcat(files[i]->name, ".chf");
		files[i]->time = 0;
		files[i]->fileptr = fopen(files[i]->name, "w");
		//printf("%s\n", PREAMBLE);
		fprintf(files[i]->fileptr, "%s", PREAMBLE);
		fprintf(files[i]->fileptr, "%s%s", filename, number);
		fprintf(files[i]->fileptr, "%s", BODY1);
		fprintf(files[i]->fileptr, "%s", BODY2);
		files[i]->noteOn = false;
		files[i]->channel = -1;
		files[i]->note = -1;
	} // initialize LBFile objects. Unused objects will be deleted after termination of program.
	size_t numFiles = 0;
	if (mode == 0b0000) 
	{
		printf("MIDI file is format 0.\n");
		numFiles = ParseModeZero(files, 0, LBARRAY_SIZE, midi, divisions, debugPtr);
	} // enter single track mode
	else if (mode == 0b0001)
	{
		printf("MIDI file is format 1.\n");
		numFiles = ParseModeOne(files, LBARRAY_SIZE, midi, numtrack, divisions, debugPtr);
	} // enter simultaneous track mode
	else if (mode == 0b0010)
	{
		printf("MIDI file uses sequential track mode, which is not yet supported.\n");
		exit(EXIT_FAILURE);
	} // sequential track mode
	fclose(midi);
	fclose(debugPtr);
	printf("Enter how many music files you wish to generate (%zu max): ", numFiles);
	int arg = 0;
	scanf("%d", &arg);
	while (!(arg > 0 && arg <= numFiles))
	{
		printf("Invalid Argument.\n");
		printf("Enter how many music files you wish to generate (%zu max): ", numFiles);
		scanf("%d", &arg);
	}
	char playName[80] = "Play";
	strcat(playName, filename);
	strcat(playName, ".chf");
	FILE* playPtr = fopen(playName, "w");
	fprintf(playPtr, "#include <linkbot.h>\n");
	for (size_t i = 0; i < numFiles && i < arg; i++)
		fprintf(playPtr, "#include \"%s\"\n", files[i]->name);
	for (size_t i = 0; i < numFiles && i < arg; i++)
		fprintf(playPtr, "CLinkbotI robot%zu;\n", i + 1);
	for (size_t i = 0; i < numFiles - 1 && i < arg - 1; i++)
		fprintf(playPtr, "robot%zu.playMelodyNB(%s, 1);\n", i + 1, files[i]->functionName);
	fprintf(playPtr, "robot%d.playMelody(%s, 1);\n", arg, files[arg-1]->functionName);
	for (size_t i = 0; i < numFiles - 1 && i < arg - 1; i++)
		fprintf(playPtr, "robot%zu.playNotesWait();\n", i + 1);
	fclose(playPtr);
	for (size_t i = 0; i < LBARRAY_SIZE; i++)
	{
		fprintf(files[i]->fileptr, "%s", BODY3);
		fclose(files[i]->fileptr);
		if (!files[i]->editted)
			remove(files[i]->name);
		free(files[i]);
	}
	return 0;
}