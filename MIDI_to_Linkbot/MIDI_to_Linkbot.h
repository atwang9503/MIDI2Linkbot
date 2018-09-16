/*
 * MIDI_TO_LINKBOT.h -  A collection of macros and constants for MIDI programming.
 */

#ifndef _MIDI_TO_LINKBOT_H
#define _MIDI_TO_LINKBOT_H

typedef enum {
		msgNoteOff				= 0x80,		/* [ pitch, volume ] */
		msgNoteOn				= 0x90,		/* [ pitch, volume ] */
		msgKeyPressure			= 0xa0,		/* [ pitch, pressure ]*/
		msgSetParameter			= 0xb0,		/* [ param number (CC), setting ] */
		msgSetProgram			= 0xc0,		/* [ program ] */
		msgChangePressure		= 0xd0,		/* [ pressure (after touch) ] */
		msgSetPitchWheel		= 0xe0,		/* [ LSB,  MSG (two 7 bit values) ] */
		
		msgMetaEvent			= 0xff,
		msgSysEx1				= 0xf0,
		msgSysEx2				= 0xf7,

		/* Alternative names */
		msgPatchChange			= msgSetProgram,
		msgControlChange		= msgSetParameter,

		msgSysMask				= 0xf0,
		} tMIDI_MSG;

typedef enum {
		keyCFlatMaj				= 0x87,
		keyGFlatMaj				= 0x86,
		keyDFlatMaj				= 0x85,
		keyAFlatMaj				= 0x84,
		keyEFlatMaj				= 0x83,
		keyBFlatMaj				= 0x82,
		keyFMaj					= 0x81,
		keyCMaj					= 0x00,
		keyGMaj					= 0x01,
		keyDMaj					= 0x02,
		keyAMaj					= 0x03,
		keyEMaj					= 0x04,
		keyBMaj					= 0x05,
		keyFSharpMaj			= 0x06,
		keyCSharpMaj			= 0x07,
		keyCFlatMin				= 0xc7,
		keyGFlatMin				= 0xc6,
		keyDFlatMin				= 0xc5,
		keyAFlatMin				= 0xc4,
		keyEFlatMin				= 0xc3,
		keyBFlatMin				= 0xc2,
		keyFMin					= 0xc1,
		keyCMin					= 0x40,
		keyGMin					= 0x41,
		keyDMin					= 0x42,
		keyAMin					= 0x43,
		keyEMin					= 0x44,
		keyBMin					= 0x45,
		keyFSharpMin			        = 0x46,
		keyCSharpMin			= 0x47,
		keyMaskNeg				= 0x80,
		keyMaskFlatKeys			= 0x80,
		keyMaskMin				= 0x40,
		keyMaskKey				= 0x07,
		} tMIDI_KEYSIG;
typedef enum {
		metaSequenceNumber		= 0,	
		metaTextEvent			= 1,
		metaCopyright			= 2,
		metaTrackName			= 3,
		metaInstrument			= 4,
		metaLyric				= 5,
		metaMarker				= 6,
		metaCuePoint			= 7,
		metaMIDIPort			= 0x21, 	
		metaEndSequence			= 0x2f,		/* followed by zero */
		metaSetTempo			= 0x51,		
		metaSMPTEOffset			= 0x54,		
		metaTimeSig				= 0x58,		
		metaKeySig				= 0x59,		
		} tMIDI_META;	
	

#define MIDI_WHEEL_CENTRE			8192
#define MIDI_CHANNEL_1			1
#define MIDI_CHANNEL_2			2
#define MIDI_CHANNEL_3			3
#define MIDI_CHANNEL_4			4
#define MIDI_CHANNEL_5			5
#define MIDI_CHANNEL_6			6
#define MIDI_CHANNEL_7			7
#define MIDI_CHANNEL_8			8
#define MIDI_CHANNEL_9			9
#define MIDI_CHANNEL_10			10
#define MIDI_CHANNEL_11			11
#define MIDI_CHANNEL_12			12
#define MIDI_CHANNEL_13			13
#define MIDI_CHANNEL_14			14
#define MIDI_CHANNEL_15			15
#define MIDI_CHANNEL_16			16
#define MIDI_CHANNEL_DRUMS		10
#define MIDI_OCTAVE			12

#define MIDI_NOTE_MIDDLE_C	MIDI_NOTE_C5

#define MIDI_NOTE_C			0
#define MIDI_NOTE_C_SHARP	1
#define MIDI_NOTE_C_FLAT	-11
#define MIDI_NOTE_D			2
#define MIDI_NOTE_D_SHARP	3
#define MIDI_NOTE_D_FLAT	1
#define MIDI_NOTE_E			4
#define MIDI_NOTE_E_SHARP	5
#define MIDI_NOTE_E_FLAT	3
#define MIDI_NOTE_F			5
#define MIDI_NOTE_F_SHARP	6
#define MIDI_NOTE_F_FLAT	5
#define MIDI_NOTE_G			7
#define MIDI_NOTE_G_SHARP	8
#define MIDI_NOTE_G_FLAT	6
#define MIDI_NOTE_A			9
#define MIDI_NOTE_A_SHARP	10
#define MIDI_NOTE_A_FLAT	8
#define MIDI_NOTE_B			11
#define MIDI_NOTE_B_SHARP	12
#define MIDI_NOTE_B_FLAT	10

#define MIDI_NOTE_C0		0 
#define MIDI_NOTE_C1		12
#define MIDI_NOTE_C2		24
#define MIDI_NOTE_C3		36
#define MIDI_NOTE_C4		48
#define MIDI_NOTE_C5		60
#define MIDI_NOTE_C6		72
#define MIDI_NOTE_C7		84
#define MIDI_NOTE_C8		96
#define MIDI_NOTE_C9		108
#define MIDI_NOTE_C10		120

#define MIDI_OCTAVE_0		MIDI_NOTE_C0
#define MIDI_OCTAVE_1		MIDI_NOTE_C1
#define MIDI_OCTAVE_2		MIDI_NOTE_C2
#define MIDI_OCTAVE_3		MIDI_NOTE_C3
#define MIDI_OCTAVE_4		MIDI_NOTE_C4
#define MIDI_OCTAVE_5		MIDI_NOTE_C5
#define MIDI_OCTAVE_6		MIDI_NOTE_C6
#define MIDI_OCTAVE_7		MIDI_NOTE_C7
#define MIDI_OCTAVE_8		MIDI_NOTE_C8
#define MIDI_OCTAVE_9		MIDI_NOTE_C9
#define MIDI_OCTAVE_10		MIDI_NOTE_C10

#define MIDI_NOTE_BREVE					1536
#define MIDI_NOTE_MINIM					 768
#define MIDI_NOTE_CROCHET				 384
#define MIDI_NOTE_QUAVER				 192
#define MIDI_NOTE_SEMIQUAVER			  96
#define MIDI_NOTE_SEMIDEMIQUAVER		  48

#define MIDI_NOTE_DOTTED_MINIM			 (768+384)
#define MIDI_NOTE_DOTTED_CROCHET		 (384+192)
#define MIDI_NOTE_DOTTED_QUAVER			 (192+96)
#define MIDI_NOTE_DOTTED_SEMIQUAVER		  (96+48)
#define MIDI_NOTE_DOTTED_SEMIDEMIQUAVER	  (48+24)

#define MIDI_NOTE_TRIPLE_CROCHET			256			

#define MIDI_VOL_FULL		127
#define MIDI_VOL_HALF		64
#define MIDI_VOL_OFF		0

#define MIDI_PATCH_ACOUSTIC_GRAND_PIANO         0 
#define MIDI_PATCH_BRIGHT_ACOUSTIC_PIANO        1
#define MIDI_PATCH_ELECTRIC_GRAND_PIANO         2
#define MIDI_PATCH_HONKY_TONK_PIANO             3
#define MIDI_PATCH_ELECTRIC_PIANO_1				4
#define MIDI_PATCH_ELECTRIC_PIANO_2             5
#define MIDI_PATCH_HARPSICHORD                  6
#define MIDI_PATCH_CLAVI						7
#define MIDI_PATCH_CELESTA   					8
#define MIDI_PATCH_GLOCKENSPIEL					9
#define MIDI_PATCH_MUSIC_BOX					10
#define MIDI_PATCH_VIBRAPHONE					11
#define MIDI_PATCH_MARIMBA						12
#define MIDI_PATCH_XYLOPHONE       				13
#define MIDI_PATCH_TUBULAR_BELLS                14
#define MIDI_PATCH_DULCIMER						15
#define MIDI_PATCH_DRAWBAR_ORGAN                16
#define MIDI_PATCH_PERCUSSIVE_ORGAN             17
#define MIDI_PATCH_ROCK_ORGAN					18
#define MIDI_PATCH_CHURCH_ORGAN					19
#define MIDI_PATCH_REED_ORGAN					20
#define MIDI_PATCH_ACCORDION					21
#define MIDI_PATCH_HARMONICA					22
#define MIDI_PATCH_TANGO_ACCORDION              23
#define MIDI_PATCH_ACOUSTIC_GUITAR_NYLON        24
#define MIDI_PATCH_ACOUSTIC_GUITAR_STEEL        25
#define MIDI_PATCH_ELECTRIC_GUITAR_JAZZ         26
#define MIDI_PATCH_ELECTRIC_GUITAR_CLEAN        27
#define MIDI_PATCH_ELECTRIC_GUITAR_MUTED        28
#define MIDI_PATCH_OVERDRIVEN_GUITAR			29
#define MIDI_PATCH_DISTORTION_GUITAR			30
#define MIDI_PATCH_GUITAR_HARMONICS				31
#define MIDI_PATCH_ACOUSTIC_BASS				32
#define MIDI_PATCH_ELECTRIC_BASS_FINGER			33
#define MIDI_PATCH_ELECTRIC_BASS_PICK			34
#define MIDI_PATCH_FRETLESS_BASS                35
#define MIDI_PATCH_SLAP_BASS_1                  36
#define MIDI_PATCH_SLAP_BASS_2                  37
#define MIDI_PATCH_SYNTH_BASS_1                 38
#define MIDI_PATCH_SYNTH_BASS_2					39
#define MIDI_PATCH_VIOLIN						40
#define MIDI_PATCH_VIOLA						41
#define MIDI_PATCH_CELLO 						42
#define MIDI_PATCH_CONTRABASS            		43
#define MIDI_PATCH_TREMOLO_STRINGS         		44
#define MIDI_PATCH_PIZZICATO_STRINGS			45
#define MIDI_PATCH_ORCHESTRAL_HARP				46
#define MIDI_PATCH_TIMPANI        				47
#define MIDI_PATCH_STRING_ENSEMBLE_1			48
#define MIDI_PATCH_STRING_ENSEMBLE_2            49
#define MIDI_PATCH_SYNTHSTRINGS_1               50
#define MIDI_PATCH_SYNTHSTRINGS_2               51
#define MIDI_PATCH_CHOIR_AAHS					52
#define MIDI_PATCH_VOICE_OOHS					53
#define MIDI_PATCH_SYNTH_VOICE					54
#define MIDI_PATCH_ORCHESTRA_HIT				55
#define MIDI_PATCH_TRUMPET						56
#define MIDI_PATCH_TROMBONE						57
#define MIDI_PATCH_TUBA							58
#define MIDI_PATCH_MUTED_TRUMPET				59
#define MIDI_PATCH_FRENCH_HORN					60
#define MIDI_PATCH_BRASS_SECTION				61
#define MIDI_PATCH_SYNTHBRASS_1					62
#define MIDI_PATCH_SYNTHBRASS_2					63
#define MIDI_PATCH_SOPRANO_SAX					64
#define MIDI_PATCH_ALTO_SAX						65
#define MIDI_PATCH_TENOR_SAX           			66
#define MIDI_PATCH_BARITONE_SAX        			67
#define MIDI_PATCH_OBOE            				68
#define MIDI_PATCH_ENGLISH_HORN    		  		69
#define MIDI_PATCH_BASSOON            			70
#define MIDI_PATCH_CLARINET            			71
#define MIDI_PATCH_PICCOLO              		72
#define MIDI_PATCH_FLUTE              			73
#define MIDI_PATCH_RECORDER           		    74
#define MIDI_PATCH_PAN_FLUTE					75
#define MIDI_PATCH_BLOWN_BOTTLE				    76
#define MIDI_PATCH_SHAKUHACHI					77
#define MIDI_PATCH_WHISTLE						78
#define MIDI_PATCH_OCARINA					    79
#define MIDI_PATCH_LEAD_1_SQUARE	            80
#define MIDI_PATCH_LEAD_2_SAWTOOTH				81
#define MIDI_PATCH_LEAD_3_CALLIOPE				82
#define MIDI_PATCH_LEAD_4_CHIFF					83
#define MIDI_PATCH_LEAD_5_CHARANG				84
#define MIDI_PATCH_LEAD_6_VOICE 				85
#define MIDI_PATCH_LEAD_7_FIFTHS				86
#define MIDI_PATCH_LEAD_8_BASS_AND_LEAD			87
#define MIDI_PATCH_PAD_1_NEW_AGE				88
#define MIDI_PATCH_PAD_2_WARM					89
#define MIDI_PATCH_PAD_3_POLYSYNTH				90
#define MIDI_PATCH_PAD_4_CHOIR					91
#define MIDI_PATCH_PAD_5_BOWED					92
#define MIDI_PATCH_PAD_6_METALLIC				93
#define MIDI_PATCH_PAD_7_HALO					94
#define MIDI_PATCH_PAD_8_SWEEP					95
#define MIDI_PATCH_FX_1_RAIN					96
#define MIDI_PATCH_FX_2_SOUNDTRACK				97
#define MIDI_PATCH_FX_3_CRYSTAL					98
#define MIDI_PATCH_FX_4_ATMOSPHERE				99
#define MIDI_PATCH_FX_5_BRIGHTNESS				100
#define MIDI_PATCH_FX_6_GOBLINS					101
#define MIDI_PATCH_FX_7_ECHOES					102
#define MIDI_PATCH_FX_8_SCIFI					103
#define MIDI_PATCH_SITAR						104
#define MIDI_PATCH_BANJO						105
#define MIDI_PATCH_SHAMISEN						106
#define MIDI_PATCH_KOTO							107
#define MIDI_PATCH_KALIMBA						108
#define MIDI_PATCH_BAG_PIPE						109
#define MIDI_PATCH_FIDDLE						110
#define MIDI_PATCH_SHANAI						111

#define MIDI_DRUM_ACOUSTIC_BASS_DRUM		35
#define MIDI_DRUM_BASS_DRUM					36
#define MIDI_DRUM_SIDE_STICK				37
#define MIDI_DRUM_ACOUSTIC_SNARE			38
#define MIDI_DRUM_HAND_CLAP					39
#define MIDI_DRUM_ELECTRIC_SNARE			40
#define MIDI_DRUM_LOW_FLOOR_TOM				41
#define MIDI_DRUM_CLOSED_HI_HAT				42
#define MIDI_DRUM_HIGH_FLOOR_TOM			43
#define MIDI_DRUM_PEDAL_HI_HAT				44
#define MIDI_DRUM_LOW_TOM					45
#define MIDI_DRUM_OPEN_HI_HAT				46
#define MIDI_DRUM_LOW_MID_TOM				47
#define MIDI_DRUM_HI_MID_TOM				48
#define MIDI_DRUM_CRASH_CYMBAL_1			49
#define MIDI_DRUM_HIGH_TOM					50
#define MIDI_DRUM_RIDE_CYMBAL_1				51
#define MIDI_DRUM_CHINESE_CYMBAL			52
#define MIDI_DRUM_RIDE_BELL					53
#define MIDI_DRUM_TAMBOURINE				54
#define MIDI_DRUM_RIDE_CYMBAL_2				59
#define MIDI_DRUM_HI_BONGO					60
#define MIDI_DRUM_LOW_BONGO					61
#define MIDI_DRUM_MUTE_HI_CONGA				62
#define MIDI_DRUM_OPEN_HI_CONGA				63
#define MIDI_DRUM_LOW_CONGA					64
#define MIDI_DRUM_HIGH_TIMBALE				65
#define MIDI_DRUM_LOW_TIMBALE				66
#define MIDI_DRUM_HIGH_AGOGO				67
#define MIDI_DRUM_LOW_AGOGO					68
#define MIDI_DRUM_CABASA					69
#define MIDI_DRUM_MARACAS					70
#define MIDI_DRUM_SHORT_WHISTLE				71
#define MIDI_DRUM_LONG_WHISTLE				72
#define MIDI_DRUM_SHORT_GUIRO				73
#define MIDI_DRUM_LONG_GUIRO				74
#define MIDI_DRUM_CLAVES					75
#define MIDI_DRUM_HI_WOOD_BLOCK				76
#define MIDI_DRUM_LOW_WOOD_BLOCK			77
#define MIDI_DRUM_MUTE_CUICA				78
#define MIDI_DRUM_OPEN_CUICA				79
#define MIDI_DRUM_MUTE_TRIANGLE				80
#define MIDI_DRUM_OPEN_TRIANGLE				81
// Credit: https://github.com/MarquisdeGeek/midilib/blob/master/src/midiinfo.h
// LINKBOT FILE INFORMATION

char* PREAMBLE = \
"#include <linkbot.h>\n"
"CLinkbotI robot;\n"
"note_t ";

char* BODY1 = \
" (int i) {\n"
"\tint len;\n"
"\tnote_t note;\n";
char* BODY2 = \
"\tnote_t song[] = {\n";

char* BODY3 = \
"\t};\n"
"\tlen = sizeof(song) / sizeof(note_t);\n"
"\tif (i < len) {\n"
"\tnote.frequency = song[i].frequency;\n"
"\tnote.duration = song[i].duration;\n"
"\t} else {\n"
"\tnote.frequency = -1;\n"
"\tnote.duration = -1;\n"
"\t}\n"
"\treturn note;\n"
"}\n";

char* NOTES[] = {
	"C1", "CS1", "D1", "DS1", "E1", "F1", "FS1", "G1", "GS1", "A1", "AS1", "B1", "C2", "CS2", "D2", "DS2", "E2", "F2", "FS2", "G2", "GS2", "A2", "AS2", "B2",
	"C3", "CS3", "D3", "DS3", "E3", "F3", "FS3", "G3", "GS3", "A3", "AS3", "B3", "C4", "CS4", "D4", "DS4", "E4", "F4", "FS4", "G4", "GS4", "A4", "AS4", "B4",
	"C5", "CS5", "D5", "DS5", "E5", "F5", "FS5", "G5", "GS5", "A5", "AS5", "B5", "C6", "CS6", "D6", "DS6", "E6", "F6", "FS6", "G6", "GS6", "A6", "AS6", "B6",
	"C7", "CS7", "D7", "DS7", "E7", "F7", "FS7", "G7", "GS7", "A7", "AS7", "B7", "C8", "CS8", "D8", "DS8"
}; // Defines the notes being played as strings that go into the .chf file

char* NOTES2[] = {
	"16.351", "17.324", "18.354", "19.445", "20.601", "21.827", "23.124", "24.499", "25.956", "27.5", "29.135", "30.868",
	"32.703", "34.648", "36.708", "38.891", "41.203", "43.654", "46.249", "48.999", "51.913", "55", "58.27", "61.735",
	"65.406", "69.296", "73.416", "77.782", "82.407", "87.307", "92.499", "97.999", "103.826", "110", "116.541", "123.471",
	"130.813", "138.591", "146.832", "155.563", "164.814", "174.614", "184.997", "195.998", "207.652", "220", "233.082", "246.942",
	"261.626", "277.183", "293.665", "311.127", "329.628", "349.228", "369.994", "391.995", "415.305", "440", "466.164", "493.883",
	"523.251", "554.365", "587.33", "622.254", "659.255", "698.456", "739.989", "783.991", "830.609", "880", "932.328", "987.767",
	"1046.502", "1108.731", "1174.659", "1244.508", "1318.51", "1396.913", "1479.978", "1567.982", "1661.219", "1760", "1864.655", "1975.533",
	"2093.005", "2217.461", "2349.318", "2489.016", "2637.021", "2793.826", "2959.955", "3135.964", "3322.438", "3520", "3729.31", "3951.066",
	"4186.009", "4434.922", "4698.636", "4978.032", "5274.042", "5587.652", "5919.91", "6271.928", "6644.876", "7040", "7458.62", "7902.132",
	"8372.018", "8869.844", "9397.272", "9956.064", "10548.084", "11175.304", "11839.82", "12543.856", "13289.752", "14080", "14917.24", "15804.264"
};

// FUNCTION PROTOTYPES
typedef struct LBFile
{
	bool editted; // false by default, set to true when parser touches this file
	char name[80]; // holds the filename of the file
	FILE* fileptr; // points to the file that the object writes to
	unsigned long int time; //  Keeps track of most recent time used in file
	bool noteOn; // NoteOn has been written to file with no termination yet
	int channel; // the note's channel as specified by the MIDI file.
	int note; // number denotes the note being played.
} LBFile;
char* IntToString(char* string, int i);
void IntToString_helper(char* string, int i, int* places);
unsigned long int variableQuantity(FILE* fileptr);
void variableQuantity_helper(FILE* fileptr, unsigned long int* value);
unsigned long int readBytes(unsigned char* buffer, FILE* fileptr, size_t num);
int PlaceNote(LBFile** files, size_t startIndex, size_t filesSize, unsigned long int absoluteTime, int noteNumber, int channelNumber, const unsigned int divisions);
int EndNote(LBFile** files, size_t startIndex, size_t filesSize, unsigned long int absoluteTime, int noteNumber, int channelNumber, const unsigned int divisions);
long int SkipSystemCommon(FILE* midi);
long int SkipSystemRealtime(FILE* midi);
size_t ParseModeZero(LBFile** files, size_t startIndex, size_t filesSize, FILE* midi, const unsigned int divisions);
size_t ParseModeOne(LBFile** files, size_t filesSize, FILE* midi, const unsigned int numtrack, const unsigned int divisions);
const char *get_filename_ext(const char *filename);
char* removeSpaces(char *str);
#endif	/* _MIDI_2_LINKBOT_H */


