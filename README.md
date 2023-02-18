# Gamma Bloke: An attempt in creating a simple GameBoy emulator, only in C
## Context
So I am mostly a C++ programmer that has dabbled in similar C/C++ programming languages (Obj-C++, Odin etc). However, I never really got into programming with C only as I thought it was "too old", or that "C++ is just better". Basically any flimsy excuse for not wanting to try out a language. Well, after almost 2 years of full C++ development I've come to grasp that I might enjoy programming in C more than in C++. Why? Because over the months I've noticed that my views on writing clean and readable code aligns more with C's thinking rather than C++'s. While I love C++'s wide featureset and freedom, it's a little too much and after awhile working in a C++ codebase becomes tiresome and you just wanna move on something else (cough cough HPL).

This project was originally just gonna be my 3rd revival of trying to create a GameBoy emulator, no C-only gimmick or anything. However only a few minutes in I realized that I was once again writing code from the C API and not C++'s. So I decided to make this a little more interesting by finally giving C a fair shot - creating the entire emulator in C only.

## The rules of this challenge
- Can only use C for the actual source code (cannot link C++/Obj-C code through libraries or through any other weird and hacky means).
- I can only use C89 (no C99, C11, C23 etc. - just the first standardized version of C).
- Minimal to no 3rd-party dependecies, apart from necessary APIs like Win32 or OpenGL.

# Building & compiling
Just download the repo and run `make` in the terminal.
## A few notes on compiling the source.
In `source/main.c`, there are 2 define modes that you can set to 0 or 1. Both macros contain a comment explaining what they enable/disable.

# Update Log & Diary on using C
## Second release (2023-02-18)
Day 4 on this project. So far I don't have that many complaints with C89 (apart from forcing me to use `/* */` for comments and loops), but I guess that's mainly due to the fact that I am studying more on how to implement a GameBoy emulator rather than learning the ins and outs of C. Anyhow, I've implemented enough opcodes to fully boot into the GameBoy's startup ROM. I also now have implemented a really basic (and broken) PPU that barely displays anything, but it's a start regardless.

## First release (2023-02-14)
So far I've implemented a CPU, ROM and GameBoy structure. The CPU is mostly barebones, you can get the memory and name of the ROM and I've implemented the first five GameBoy opcodes (0x00-0x05) into the program.

Honestly this has been quite an enjoyable experience. Of course the project is really simple and doesn't have any complex moving parts. What caught me off-guard the most is how fast C compiles compared to C++. I kinda expected that C would compile faster but not by **this** much. I did find some things annoying, like how I have to use `typedef` to normally define structs like how you would in C++, not being able to do `for` loops normally like in C++ or newer versions of C (eg. `for (int i = 0; i < 10; i++)` isn't allowed) and other minor stuff that I've already forgoten about so ig they don't really count.

Overall, really excited to continue this! I didn't really have to change much of my "mindset" so to speak with switching from C++ to C. But of course, I've only written like a hundred lines of code so this could go horribly wrong at any moment. I guess we'll just have to wait and see.

# Credits
## Other people who helped/did stuff
King Galactix#6509 - for suggesting the very kool and epic name of Gamma Bloke.</br >
[ps4star](https://github.com/ps4star) - kinda making me to try out C after his 24378935472745893rd rant about why C++ baed, odin gud. Also helping me out with the development of this emulator by answering my questions and checking if some of my emulation implementations are correct.
## Resources used for this project
For anyone else attempting to make a GameBoy emulator, I would highly recommend to visit these resources:
### General GameBoy information
- [Pan Docs](https://gbdev.io/pandocs/) - the most comprehensive GameBoy technical documentation, covering pretty much everything GameBoy related (CPU, PPU, sound etc).
- [Game Boy: Complete Technical Reference](https://gekkio.fi/files/gb-docs/gbctr.pdf) - as the name says, it's a complete technical reference in PDF format. Covers everything and even includes pseudocode for a lot of the opcodes.
- [Game Boy CPU Manual](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf) - a good general reference, containing great explanations for everything CPU related.

### CPU/Opcodes
- [Pan Docs: 13. CPU Registers and Flags](https://gbdev.io/pandocs/CPU_Registers_and_Flags.html) - short and descriptive CPU registers and flags explanations.
- [gbops](https://izik1.github.io/gbops/) - a simple to understand and accurate opcode table for the GameBoy. Contains every GameBoy opcode, their hex values and results.
- [clrhome Z80 table](https://clrhome.org/table) - a Z80 opcode table. Since the GameBoy CPU is heavilly based on the Z80, a lot of the time they both share the same opcodes. What's also important to note is that unlike gbops, this table actually contains descriptions for every opcode, making it very useful and easy to learn about what every opcode does in practice.
- [gbz80](https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7) - a list of GameBoy opcodes, containing explanations of what they do and their results in practice.

### Graphics
- [Pan Docs: 4. Rendering (and it's lower points)](https://gbdev.io/pandocs/Rendering.html) - a must-read for learning about graphics for the GameBoy.
- [Game Boy CPU Manual: 2.8. Video](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf) - less comprehensive than Pan Docs' descriptions but easier to grasp and understand.
- [Marcus Huderle's gameboy2bpp demo](https://www.huderlem.com/demos/gameboy2bpp.html) - simple to understand explanation about how GameBoy tiles are rendered. It also contains a demo of the 2BPP graphics format, useful for checking if your implementation of the rendering is correct.

### Hardware registers
- [Pan Docs: 3. Summary](https://gbdev.io/pandocs/Hardware_Reg_List.html) - a list containing all hardware registers.