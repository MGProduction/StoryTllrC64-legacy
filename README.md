# StoryTllrC64
A small C interactive fiction engine for C64 [it's currently a work-in-progress]

# Usage
This engine works on the compiled data generate from scripts in a specific proprietary language (see LANGSPEC.md)

Compiled data can be used as binary file or a .h file - storytllr64_data.h. 

If you compile the scripts in binary format then you just need to create a .d64 disk image with the engine, the game data (advcartridge) and the other image files.

If you choose to compile the scripts as .h (data for the C project) you need to rebuild the project itsself, and the create the disk with the resulting .prg and the other image files.

