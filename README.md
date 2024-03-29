# MIDI2Linkbot
A program that generates music files playable by Linkbots by reading in MIDI files.
## How to use
After compiling with your favorite C compiler, place the MIDI_to_Linkbot.exe file in the folder with the .mid file you wish to generate .chf files for. The current build of the program supports format 0/1 MIDI files (most MIDI files fall in this category) which use ticks to represent delta time. SMPTE formate MIDI files are not yet supported (can be implemented later easily). 

Usage: `./MIDI_to_Linkbot.exe [.mid file]`

The program will generate a variable number of files which together make up the entire song. Format 0 MIDI files generally have fewer track files since there will only be multiple melody files if there are simulataneous notes being played. The quantity of melody files generated by format 1 MIDI files depends on both the number of tracks and the amount of simulataneous notes in each track.

You will then be prompted to enter the number of robots you will be using to play the song.

`Enter how many music files you wish to generate (n max):`

After entering some number `0 < k <= n` The program will then generate a .chf that runs the melody functions created from 1st to kth.

No melody files generated will play anything played by percussion instruments.

### Notes
* Usually the main melody for format 1 MIDI files is the first track, so the first melody file will generally have the main melody of the song. The rest of the files are usually harmony melody files. 
* Higher numbered melody files will generally contain the background track, or very sparse notes that could not fit on the main melody/track files due to simultaneous notes being played at once. Usually these do not impact the songs greatly and can be ignored.

## References that helped a lot
1. http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
2. http://flashmusicgames.com/midi/mid2txt.php
3. https://www.csie.ntu.edu.tw/~r92092/ref/midi/
4. https://www.recordingblogs.com/wiki/musical-instrument-digital-interface-midi
