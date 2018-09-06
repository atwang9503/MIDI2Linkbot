/*
 * MIDI_TO_LINKBOT.h -  A collection of macros and constants for MIDI programming.
 */

#ifndef _MIDI_TO_LINKBOT_H
#define _MIDI_TO_LINKBOT_H

typedef enum {
		msgNoteOff				= 0x80,		/* [ pitch, volume ] */
		msgNoteOn				= 0x90,		/* [ pitch, volume ] */
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

#endif	/* _MIDI_2_LINKBOT_H */


