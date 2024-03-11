# Termtrove

Termtrove(or Terminal Trove) is a Terminal User Interfaced(TUI) file explorer written in C and designed for Linux machines.

Its minimalistic design and intuitive commands make the program very easy to learn and use for daily tasks.

Be aware this is an ongoing project and many common features on most file explorers are yet not implemented as:

* Create directories
* Rename files/directories
* Copy, paste or cut files/directories

## How to build it?

### Requirements

* gcc compiler;
* GNU [Make][].

### Steps

First you should clone this repository using:

`git clone https://github.com/th3worst4/termtrove.git termtrove/ && cd termtrove/`

After, you compile it using:

`make release`

A new folder called bin should appear, the compiled binary is under the path:

`bin/main`

## License

This software is under MIT License, for more information check [License][] text file.

## Update logs

* 03/09/2024 - goparent and gochild functions modified. Splited code on many source files to be more consistent

* 03/10/2024 - Updated README.md, short description and HowTo build were added. Added delete function.

[License]:/LICENSE
[Make]: https://www.gnu.org/software/make/manual/make.html
