# CMatrix

CMatrix is based on the screensaver from The Matrix website. It shows text
flying in and out in a terminal like as seen in "The Matrix" movie. It can
scroll lines all at the same rate or asynchronously and at a user-defined
speed.

CMatrix by default operates in **eye candy** mode.  It must be aborted with
control-c (Ctrl+C) or by pressing q.  If you wish for more of a screen saver
effect, you must specify `-s` on the command line. For usage info, use `cmatrix -h`.

[![Build Status](https://travis-ci.org/abishekvashok/cmatrix.svg?branch=master)](https://travis-ci.org/abishekvashok/cmatrix)

### Dependencies
You'll probably need a decent ncurses library (or PDCurses on native Windows) to get this to work.

### Building and installing cmatrix
To install cmatrix, use either of the following methods from within the cmatrix directory.

#### Using `configure` (recommended for most linux user)
```
autoreconf -i  # skip if using released tarball
./configure
make
make install
```

#### Using CMake
Here we also show an out-of-source build in the sub directory "build".
Don't use CMake if you want to use PDCurses, it won't work (for now).
```
mkdir -p build
cd build
# to install to "/usr/local"
cmake ..
# or to install to "/usr"
#cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
```

### Running cmatrix
After you have installed cmatrix just run `cmatrix` to run cmatrix :)

_To get the program to look most like the movie, use `cmatrix -lba`_
_To get the program to look most like the Win/Mac screensaver, use `cmatrix -ol`_

### Valuable information
If you have any suggestions/flames/patches to send, please feel free to
open issues and if possible solve them in PRs via Github.

_Note: cmatrix is probably not particularly portable or efficient, but it wont hog
**too** much CPU time_

### Captures

#### Screenshots

![Special Font & bold](data/img/capture_bold_font.png?raw=true "cmatrix -bx")

#### Screencasts

![Movie-Like Cast](data/img/capture_orig.gif?raw=true "cmatrix -xba")

### Maintainers
- Abishek V Ashok (@abishekvashok) <abishekvashok@gmail.com> [Core]

### Thanks to:
- Chris Allegretta <chrisa@asty.org> for writing cmatrix up in a fornight and giving us
  the responsibility to further improve it.
- Krisjon Hanson and Bjoern Ganslandt for helping with bold support and 
  Bjoern again for the cursor removal code, helping with the `-u` and `-l`
  modes/flags, and Makefile improvements.
- Adam Gurno for multi-color support.
- Garrick West for debian consolefont dir support.
- Nemo for design thoughts and continuous help and support.
- John Donahue for helping with transparent term support
- Ben Esacove for Redhat 6 compatibility w/matrix.psf.gz
- jwz for the xmatrix module to xscreensaver at http://www.jwz.org/xscreensaver.
- Chris Allegretta's girlfriend Amy for not killing him when he stayed up till 3 AM
  writing code.
- The makers of the Matrix for one kickass movie!
- Everyone who has sent (and who will send) us and Chris mails regarding
  bugs, comments, patches or just a hello.
- Everyone who has opened issues and PRs on the github repository.

### License
This software is provided under the GNU GPL v3.

### Disclaimer
We are in no way affiliated in any way with the movie "The Matrix", "Warner Bros" nor
any of its affiliates in any way, just fans.

