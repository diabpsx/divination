# Divination
Tools for interacting with the PSX file formats

### dgfx
Converts the various graphic formats to PCX files. Currently must be recompiled for HDR/DAT or HDR/TP formats.

**NOTE** this tool is still a WIP as there are several formats and some of them are tricky. The tool may crash and not work with some files.

Usage: dgfx [hdr] [dat] [pal]

- hdr: .HDR graphic header file.
- dat: .DAT graphic data file.
- pal: File to output RGB palette data.

### dtext
Converts compiled string files (e.g. MAINTXT.ENG) back into plain text files.

Usage: dtext [dat] [out]

- dat: The compiled text file.
- out: File to output the text data.

### dkanji
Dumps Kanji font files (e.g. MAINTXT.OUT) into pixel-bit text files.

Usage: dkanji [in] [out]

- in: The Kanji font file.
- out: File to output the text version of the font.

### dstream
Dumps the contents of stream files (.DIR/.BIN pair). These contain all of the game data, including binary overlays.

Usage: dstream [dir] [bin] [types...]

- dir: .DIR index file.
- bin: .BIN data file.
- types: (optional) 3 letter file extension, up to 8 allowed. only extract files with these extenstions.

Two batch scripts are provided, **dump_all.bat** to extract all files and **dump_snd.bat** to only extract sound files (VAG and BANKS).

Note that both the US and European versions of Diablo contain English, French, German, and Swedish, but do not contain Japanese. The Japanese version is also needed to extract the Japanese files.

### dbank
Dumps the contents of sound bank files (.BOF/.BNK pair). These files are embedded in the stream files, and contain sound effects stored in VAG format.

Usage: dbank [bof] [bnk]

- bof: .BOF index file.
- bnk: .BNK data file.

A batch script **dump_all.bat** is provided to extract all bank files.

### vag2wav
Converts a PlayStation VAG sound file to a WAV file. VAG files are found in both the stream and bank files.

Usage: vag2wav [vag] [wav]

- vag: .VAG sound file.
- wav: .WAV sound file.

## Compiling
A Visual Studio workspace is provided to compile the tools. Works with Visual C++ 5.0 and newer.

# Credits
- **dgfx**, **dtext**, **dstream**, and **dbank** written by GalaXyHaXz
- **vag2wav** taken from the [psxsdk](https://github.com/ColdSauce/psxsdk)
