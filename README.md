# LibOp

# Compilation

## Linux
### Requirements

### Instructions
1. Clone project
2.  `cd build && make`

## Windows
### Requirements

* [Git for Windows](https://git-scm.com/download/win) (Optional)
* [GNU Make](http://gnuwin32.sourceforge.net/packages/make.htm)
* [Mingw-w64](https://sourceforge.net/projects/mingw-w64/)

### Instructions

1. Install Git for Windows (Optional)
2. Install GNU Make
    * Add the `GnuWin32/bin` folder to PATH
3. Install Mingw-w64 with options (7.1.0, x86_64, posix, seh, 0)
    * Add the `mingw-w64/x86_64-7.1.0-posix-seh-rt_05-rev0/mingw64/bin` folder to PATH
4. Clone project using Git from Git Bash (or Windows CMD)
5. `cd build && make OS=win32`
