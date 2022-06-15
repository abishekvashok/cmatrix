<h1 align="center">CMatrix</h1>

<h3 align="center"> Matrix like effect in your terminal </h3>

</p>
<p align="center">
  <a href="https://travis-ci.org/abishekvashok/cmatrix">
    <img src="https://travis-ci.org/abishekvashok/cmatrix.svg?branch=master">
  </a>
  <a href="./COPYING">
    <img src="https://img.shields.io/github/license/abishekvashok/cmatrix?color=blue">
  </a>
  <img src="https://img.shields.io/badge/contributions-welcome-orange">
  <a href="https://github.com/abishekvashok/cmatrix/stargazers">
    <img src="https://img.shields.io/github/stars/abishekvashok/cmatrix">
  </a>
  <a href="https://github.com/abishekvashok/cmatrix/network">
    <img src="https://img.shields.io/github/forks/abishekvashok/cmatrix">
  </a>
</p>


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Contents
- [Overview](#overview)
- [Build Dependencies](#build-dependencies)
- [Building and Installation](#building-and-installing-cmatrix)
    - [Using configure (recommended)](#using-configure-(recommended-for-most-linux%2Fmingw-users))
    - [Using CMake](#using-cmake)
- [Usage](#usage)
- [Captures](#captures)
    - [Screenshots](#screenshots)
    - [Screencasts](#screencasts)
- [Maintainer](#maintainers)
    - [Contributors](#our-contributors)
- [Contribution Guide](#contribution-guide)
- [License](#license)

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :cloud: Overview

CMatrix is based on the screensaver from The Matrix website. It shows text
flying in and out in a terminal like as seen in "The Matrix" movie. It can
scroll lines all at the same rate or asynchronously and at a user-defined
speed.

CMAtrix is inspired from 'The Matrix' movie. If you haven’t seen this movie and you are a fan of computers or sci-fi in general, go see this movie!!!

> :grey_exclamation:`Disclaimer` : We are in no way affiliated in any way with the movie "The Matrix", "Warner Bros" nor
any of its affiliates in any way, just fans.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :open_file_folder: Build Dependencies
You'll probably need a decent ncurses library to get this to work. On Windows, using mingw-w64-ncurses is recommended (PDCurses will also work, but it does not support colors or bold text).
<br>
##### :small_blue_diamond: For Linux<br>
Run this command to check the version of ncurses.
```
ldconfig -p | grep ncurses
```
If you get no output then you need to install ncurses. Click below to install ncurses in Linux.
- [ncurses](https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/)

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :floppy_disk: Building and installing cmatrix
To install cmatrix, Clone this repo in your local system and use either of the following methods from within the cmatrix directory.

#### :small_blue_diamond: Using `configure` (recommended for most linux/mingw users)
```sh
autoreconf -i  # skip if using released tarball
./configure
make
make install
```

#### :small_blue_diamond: Using CMake
Here we also show an out-of-source build in the sub directory "build".
(Doesn't work on Windows, for now).
```sh
mkdir -p build
cd build
# to install to "/usr/local"
cmake ..
# OR 
# to install to "/usr"
#cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
```

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :bookmark_tabs: Usage

After you have installed **cmatrix** just type the command `cmatrix` to run it :)
```sh
cmatrix
```
Run with different arguments to get different effects.
```sh
cmatrix [-abBflohnsmVx] [-u update] [-C color]
```
Example:
```sh
cmatrix -ba -u 2 -C red
```

For more options and **help** run `cmatrix -h` <br>OR<br> Read Manual Page by running command `man cmatrix`

_To get the program to look most like the movie, use `cmatrix -lba`_
_To get the program to look most like the Win/Mac screensaver, use `cmatrix -ol`_

> :round_pushpin: _Note: cmatrix is probably not particularly portable or efficient, but it wont hog
**too** much CPU time._

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :camera: Captures

#### :small_blue_diamond: Screenshots

<!-- ![Special Font & bold](data/img/capture_bold_font.png?raw=true "cmatrix -bx") -->
<p align="center">
<img src="./data/img/capture_bold_font.png" alt="cmatrix screenshot">
</p>

#### :small_blue_diamond: Screencasts

<!-- ![Movie-Like Cast](data/img/capture_orig.gif?raw=true "cmatrix -xba") -->
<p align="center">
<img src="./data/img/capture_orig.gif" alt="cmatrix screencast">
</p>

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :zap: Maintainers
- ➤ **Abishek V Ashok** [Core] - <abishekvashok@gmail.com><br> 
<p align="center">
  <a href="https://twitter.com/abishekvashok">
    <img src="https://img.shields.io/badge/Twitter-1DA1F2?style=for-the-badge&logo=twitter&logoColor=white">
  </a>
  <a href="https://github.com/abishekvashok">
    <img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white">
  </a>
</p>


## :busts_in_silhouette: Our Contributors
#### :small_orange_diamond: Thanks to:
- ➤ **Chris Allegretta** <chrisa@asty.org> for writing cmatrix up in a fortnight and giving us
  the responsibility to further improve it.
- ➤ **Krisjon Hanson** and **Bjoern Ganslandt** for helping with bold support and
  Bjoern again for the cursor removal code, helping with the `-u` and `-l`
  modes/flags, and Makefile improvements.
- ➤ **Adam Gurno** for multi-color support.
- ➤ **Garrick West** for debian consolefont dir support.
- ➤ **Nemo** for design thoughts and continuous help and support.
- ➤ **John Donahue** for helping with transparent term support
- ➤ **Ben Esacove** for Redhat 6 compatibility w/matrix.psf.gz
- ➤ **jwz** for the xmatrix module to xscreensaver at http://www.jwz.org/xscreensaver.
- Chris Allegretta's girlfriend **Amy** for not killing him when he stayed up till 3 AM
  writing code.
- ➤ **Sumit Kumar Soni** for beautifying the README.
- The makers of the Matrix for one kickass movie!
- ➤ Everyone who has sent (and who will send) us and Chris mails regarding
  bugs, comments, patches or just a simple hello.
- ➤ Everyone who has contributed to the project by opening issues and PRs on the github repository.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :book: Contribution Guide
If you have any suggestions/flames/patches to send, please feel free to
- Open issues and if possible label them, so that it is easy to categorise features, bugs etc.
- If you solved some problems or made some valuable changes, Please open a Pull Request on Github.
- See [contributing.md](./CONTRIBUTING.md) for more details.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## :page_facing_up: License
This software is provided under the GNU GPL v3. [View License](./COPYING)

