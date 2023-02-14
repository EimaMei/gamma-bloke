# Gamma Bloke: An attempt in creating a simple GameBoy emulator, only in C
## Context
So I am mostly a C++ programmer that has dabbled in similar C/C++ programming languages (Obj-C++, Odin etc). However, I never really got into programming with C only as I thought it was "too old", or that "C++ is just better". Basically any flimsy excuse for not wanting to try out a language. Well, after almost 2 years of full C++ development I've come to grasp that I might enjoy programming in C more than in C++. Why? Because over the months I've noticed that my views on writing clean and readable code aligns more with C's thinking rather than C++'s. While I love C++'s wide featureset and freedom, it's a little too much and after awhile working in a C++ codebase becomes tiresome and you just wanna move on something else (cough cough HPL).

This project was originally just gonna be my 3rd revival of trying to create a GameBoy emulator, no C-only gimmick or anything. However only a few minutes in I realized that I was once again writing code from the C API and not C++'s. So I decided to make this a little more interesting by finally giving C a fair shot - creating the entire emulator in C only.

## The rules of this challenge
- Can only use C for the actual source code (cannot link C++/Obj-C code through libraries or through any other weird and hacky means).
- I can only use C89 (no C99, C11, C23 etc. - just the first standardized version of C).
- Minimal to no 3rd-party dependecies, apart from necessary APIs like Win32 or OpenGL.

# Building
Just download the repo and run `make` in the terminal.

# Update Log & Diary on using C
## First release (2023-02-14)
So far I've implemented a CPU, ROM and GameBoy structure. The CPU is mostly barebones, you can get the memory and name of the ROM and I've implemented the first five GameBoy opcodes (0x00-0x05) into the program.

Honestly this has been quite an enjoyable experience. Of course the project is really simple and doesn't have any complex moving parts. What caught me off-guard the most is how fast C compiles compared to C++. I kinda expected that C would compile faster but not by **this** much. I did find some things annoying, like how I have to use `typedef` to normally define structs like how you would in C++, not being able to do `for` loops normally like in C++ or newer versions of C (eg. `for (int i = 0; i < 10; i++)` isn't allowed) and other minor stuff that I've already forgoten about so ig they don't really count.

Overall, really excited to continue this! I didn't really have to change much of my "mindset" so to speak with switching from C++ to C. But of course, I've only written like a hundred lines of code so this could go horribly wrong at any moment. I guess we'll just have to wait and see.

# Credits
King Galactix#6509 - for the very kool and epic name of Gamma Bloke.</br >
[ps4star](https://github.com/ps4star) - kinda motivating me to try out C after his 24378935472745893rd rant about why C++ baed, odin gud (also giving me some tips and tricks with C).</br >
My local doctor - for telling me to stop using C++ as if I continue to, my leg will sooner or later blow off.