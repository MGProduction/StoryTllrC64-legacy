# StoryTllrC64
A small C interactive fiction engine for C64 [it's currently a work-in-progress]

# Usage
This engine works on the compiled data generate from scripts in a specific proprietary language (compiled data can be used as binary file or a .h file - storytllr64_data.h). 

If you compile the scripts in binary format then you just need to create a .d64 disk image with the engine, the game data (advcartridge) and the other image files.

If you choose to compile the scripts as .h (data for the C project) you need to rebuild the project itsself, and the create the disk with the resulting .prg and the other image files.

# StoryTllrC64 language

Script language works on simple elements: **room** (locations) **obj** (elements you can interact with) and **verb** (commands you can use to play) - plus you can use bit variables (true / false) or numeric variables (0/255). room and obj have a set of properties you can check and set

There are a limited set of instructions and a limited set of metakeywords - using them (and if you want a simple two level class derivation) it's relatively easy to create a game

## Instructions

Current allowed instructions are 

### generic

  quit
  
  start
  
  load
  
  save

### about variables

  setvar
  
  addvar
  
  set
  
  unset
  
### about ui

  clear
  
  getkey
  
  waitkey
  
  msg
  
  list
  
  showobj
  
  setroomimage
  
  setroomoverlayimage
  
### about obj / room  

  goto
  
  put
  
  setattr
  
  unsetattr
  
  setobjname  
  
  setroomname
  
### about conditions  
  
  withobj  
  
  endwith
  
  if
  
  ifis
  
  ifisin
  
  ifisroom
  
  ifkey
  
  ifnot
  
  ifobjinattr
  
  ifvar  

  else
  
  endif
  

