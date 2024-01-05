# Building

This repository contain a [`Makefile`](../Makefile) file that build automatically the project when you run the `make` command.


## Requirements

To build you need:
  - [git](https://en.wikipedia.org/wiki/Git) (to clone this repo)
  - [gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)
  - [make](https://en.wikipedia.org/wiki/Make_\(software\))

## Linux

### Debian-based distro (Ubuntu, Debian, Kali Linux...)

```sh
sudo apt install git gcc make
git clone https://github.com/craftman2868/9lang.git
cd 9lang
make
# If you want to install it:
sudo make install
```

### Other distro

```sh
# Use you package manager to install **git**, **gcc** and **make**.
git clone https://github.com/craftman2868/9lang.git
cd 9lang
make
# If you want to install it:
sudo make install
```

## Windows

Not available on Windows yet.

<!--
You need [MinGW](https://www.mingw-w64.org/downloads) (for gcc and make).

Download this repo from GitHub (use git if you installed it or download the repo as a zip file and extract it)

```sh
make
```
-->
