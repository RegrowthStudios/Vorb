[![discord](https://img.shields.io/discord/459062989094649866.svg?logo=discord "Discord")](https://discord.gg/b2bf775)
![travis](https://img.shields.io/travis/RegrowthStudios/Vorb.svg?style=flat-square&label=Linux "Travis CI")
![appveyor](https://img.shields.io/appveyor/ci/SamThePsychoticLeprechaun/vorb.svg?style=flat-square&label=Windows "AppVeyor CI")

# Vorb
This repository contains the source code for Regrowth Studios Vorb Game Engine.

## Getting Started
**NOTE**: If you are wanting to set up Vorb to work with Seed of Andromeda, 
then visit follow the intructions on the [SoA github page](https://github.com/RegrowthStudios/SoACode-Public).

This guide will walk you through setting up as a contributor to or user of the
Vorb project. There is a basic requirement of having several
packages installed prior to being able to develop. We support all three
major operating systems: Windows, Mac and Linux.

### Using
If you're intending to just use the Vorb engine, great! Be sure to check out the Vorb wiki and
feel free to add any issues you have with the engine for the awareness of the engine contributors.
* [Wiki](https://github.com/RegrowthStudios/Vorb/wiki)
* [Issues](https://github.com/RegrowthStudios/Vorb/issues)

### Contributing
Before beginning to make pull requests for the engine, please take a moment to use the following resources
to get an idea of how to contribute, what you might be able to contribute to specifically,
and to meet some of the other contributors.
* [SoA Wiki - Information on contributing.](https://github.com/RegrowthStudios/SoACode-Public/wiki)
* [Vorb Wiki - Particular details of Vorb.](https://github.com/RegrowthStudios/Vorb/wiki)
* [Issues](https://github.com/RegrowthStudios/Vorb/issues)
* [Discord](https://discord.gg/b2bf775)

### Setting Up:
**IMPORTANT**: Before following any of the instructions linked below for the platforms we support,
please do take a second to fork the repository! If you are new to GitHub, you can 
do so by clicking the "fork" button on the top right of this page.

If you have cloned the repository before forking, no worries! We can fix it, by following [these instructions](#fixing-a-pre-fork-clone).

Now we're forked, follow the link to the section on setting up for your OS of choice:
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
*  (Optional) MSVS SVC Plugin:  [MSVS Git Plugin](http://msdn.microsoft.com/en-us/library/hh850437.aspx)
*  (Optional) TortoiseGit: [tortoisegit](https://tortoisegit.org/download)

#### Setup
1. Open a dos window.
```cmd
Windows + R
cmd
```
2. Create a folder to hold the repositories
```cmd
cd c:\
mkdir -p repos
```
3. Clone the Seed of Andromeda repositories
```cmd
c:\
cd c:\repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/SoACode-Public.git soa
cd c:\repos\soa
```

### Mac

#### Prerequisites
* Compiler: [Xcode](https://developer.apple.com/xcode/)
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

#### Setup
1. Open a terminal.
```bash
cmd + space
Terminal
```
2. Create a folder to hold the repositories
```bash
mkdir ~/repos
```
3. Clone the Seed of Andromeda repositories
```bash
cd ~/repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/SoACode-Public.git soa
cd ~/repos/soa
```

### Linux

#### Prerequisites
* Compiler: gcc or clang
    * Install per your preferred operating system package control...
    * Portage:
    ```bash
    sudo emerge -DuNqa gcc   # for gcc
    sudo emerge -DuNqa clang  # for clang
    ```
    * PacMan:
    ```bash
    sudo pacman -S gcc
    sudo pacman -S clang
    ```
    * Apt:
    ```bash
    sudo apt-get install gcc
    sudo apt-get install clang
    ```
    * Yum:
    ```bash
    sudo yum install gcc
    sudo yum install clang
    ```
* Software Version Control: [Git](http://git-scm.com/downloads)
    * Portage:
    ```bash
    sudo emerge -DuNqa git
    ```
    * PacMan:
    ```bash
    sudo pacman -S git
    ```
    * Apt:
    ```bash
    sudo apt-get install git
    ```
    * Yum:
    ```bash
    sudo yum install git
    ```

#### Setup
1. Open a terminal.
2. Create a folder to hold the repositories
```bash
mkdir ~/repos
```
3. Clone the Seed of Andromeda repositories
```bash
cd ~/repos
git clone --recurse-submodules https://github.com/YOUR_GITHUB_NAME/SoACode-Public.git soa
cd ~/repos/soa
```


# Building
1. Pull latest code
```bash
cd ~/repos/soa
git checkout develop    # or whichever branch you want
git pull --recurse-submodules
```
2. Run CMake
```bash
mkdir build
cd build
cmake ../    # plus whatever options you'd like to select.
```
3. Build with CMake
```bash
cmake --build ./    # plus whatever options you'd like to select (e.g. --clean-first)
```

# Fixing a Pre-Fork Clone

So, you've accidentally cloned the repository before forking it, eh? No problem. Just run the following git commands inside of the repository and everything will be as it should be!

Firstly, if you still haven't, fork the repository!

Now you have a fork we want to set `origin` of each of your local repository (which is the default remote repository to push changes to) to your corresponding forked repository on GitHub:
```bash
# Assuming we're already inside the top-level directory of your Vorb repository.
git remote set-url origin https://github.com/YOUR_GITHUB_NAME/Vorb.git
```

That's it! It's all fixed. :)
