# Vorb
This repository contains the source code for Regrowth Studios Vorb Game Engine.

## Getting Started
**NOTE**: If you are wanting to set up Vorb to work with Seed of Andromeda, 
then visit follow the intructions on the [SoA github page](https://github.com/RegrowthStudios/SoACode-Public).

This guide will walk you through setting up as a contributor to or user of the
Vorb project. There is a basic requirement of having several
packages installed prior to being able to develop. We support all three
major operating systems: Windows, Mac and Linux.

### Contributing
Before beginning your SoA journey, please take a moment to use the following resources
to get an idea of how to contribute, what you might be able to contribute to specifically,
and to meet some of the other contributors.
* [Wiki](https://github.com/RegrowthStudios/Vorb/wiki)
* [Issues](https://github.com/RegrowthStudios/Vorb/issues)
* [Discord](https://discord.gg/b2bf775)

### Setting Up:
Before following any of the instructions linked below for the platforms we support,
please do take a second to fork the repository! If you are new to GitHub, you can 
do so by clicking the "fork" button on the top right of this page.
* [Windows](#windows)
* [Mac](#mac)
* [Linux](#linux)

### Building:
Now you have a copy of the code, and perhaps have played with it a little, why not give it
a whirl?
* [Building](#building-1)
 

## Setting Up

### Windows

#### Prerequisites
*  Compiler: [Microsoft Visual Studio 2015+](https://visualstudio.microsoft.com/)
*  Software Version Control:  [Git](http://git-scm.com/downloads)
*  MSVS SVC Plugin:  [MSVS Git Plugin](http://msdn.microsoft.com/en-us/library/hh850437.aspx)
*  CMake: [Cmake](http://www.cmake.org/download/)

#### Setup
1. Open a dos window.
```
Windows + R
cmd
```
2. Create a folder to hold the repositories
```
cd c:\
mkdir -p repos
```
3. Clone the Vorb repository
```
c:\
cd c:\repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/Vorb.git vorb
```


### Mac

#### Prerequisites
1. Compiler: [Xcode](https://developer.apple.com/xcode/)
* Software Version Control: [Git](http://git-scm.com/downloads)
    Optionally, with [Homebrew](http://brew.sh/):
    ```brew install git```
* Preferred editor: [Sublime Text](http://www.sublimetext.com/) and optional packages
    * [PackageControl](https://sublime.wbond.net/installation)
    * [CMake](https://sublime.wbond.net/packages/CMake) - CMake.tmLanguage
    * [GitGutter](https://sublime.wbond.net/packages/GitGutter) - A Sublime Text 2/3 plugin to see git diff in gutter
    * [SublimeCodeIntel](https://sublime.wbond.net/packages/SublimeCodeIntel) - Full-featured code intelligence and smart autocomplete engine
    * [SublimeLinter](https://sublime.wbond.net/packages/SublimeLinter) -- Interactive code linting framework for Sublime Text 3
    * [SublimeLinter-cpplint](https://sublime.wbond.net/packages/SublimeLinter-cpplint) -- This linter plugin for SublimeLinter provides an interface to cpplint.
    * [SublimeLinter-pep8](https://sublime.wbond.net/packages/SublimeLinter-pep8) - SublimeLinter plugin for python, using pep8.
    * [SublimeLinter-contrib-clang](https://sublime.wbond.net/packages/SublimeLinter-contrib-clang) - https://sublime.wbond.net/packages/SublimeLinter-contrib-clang

#### Installation
1. Open a terminal.
```
cmd + space
Terminal
```
2. Create a folder to hold the repository
```
mkdir ~/repos
```
3. Clone the Vorb repositories
```
cd ~/repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/Vorb.git vorb
```

### Linux

#### Prerequisites
1. Compiler: gcc or clang
    * Install per your preferred operating system package control...
    * Portage:
    ```
    sudo emerge -DuNqa gcc   # for gcc
    sudo emerge -DuNqa clang  # for clang
    ```
    * PacMan:
    ```
    sudo pacman -S gcc
    sudo pacman -S clang
    ```
    * Apt:
    ```
    sudo apt-get install gcc
    sudo apt-get install clang
    ```
    * Yum:
    ```
    sudo yum install gcc
    sudo yum install clang
    ```
* Software Version Control: [Git](http://git-scm.com/downloads)
    * Portage:
    ```
    sudo emerge -DuNqa git
    ```
    * PacMan:
    ```
    sudo pacman -S git
    ```
    * Apt:
    ```
    sudo apt-get install git
    ```
    * Yum:
    ```
    sudo yum install git
    ```

#### Setup
1. Open a terminal.
2. Create a folder to hold the repositories
```
mkdir ~/repos
```
3. Clone the Vorb repository
```
cd ~/repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/Vorb.git vorb
```

## Building
1. Pull latest code
```
cd ~/repos/soa
git checkout develop    # or whichever branch you want
git pull --recurse-submodules
```
2. Run CMake
```
mkdir build
cd build
cmake ../    # plus whatever options you'd like to select.
```
3. Build with CMake
```
cmake --build ./    # plus whatever options you'd like to select (e.g. --clean-first)
```
