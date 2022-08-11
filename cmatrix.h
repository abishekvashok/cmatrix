#ifdef HAVE_CMATRIX_CONFIG_H
#include "config.h"
#endif

#ifdef _WIN32
// https://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#ifdef _WIN32
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif
#endif

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#elif defined(HAVE_TERMIO_H)
#include <termio.h>
#endif

#ifdef __CYGWIN__
#define TIOCSTI 0x5412
#endif

#ifdef HAVE_LIBINTL_H
#define gettext_noop(S) S
#define _(ARGS) gettext_noop(ARGS)
#else
#define _(ARGS) ARGS
#endif

enum BOLD_STATE {NO_BOLD = 0, BOLD_HEADING, BOLD_RANDOM, ALL_BOLD};

void usage_charsets();
void usage() {
    printf("USAGE: cmatrix -[aAbBcfhilsSmUVxk] [-u delay] [-C color] [-t tty] [-M message]\n");
    printf("\nOPTIONS:\n");
    printf(" -a: Asynchronous scroll\n");
    printf(" -b: Bold characters on\n");
    printf(" -B: All bold characters (overrides -b)\n");
    printf(" -f: Force the linux $TERM type to be on\n");
    printf(" -i: Invert colors of headings\n");
    printf(" -L: Lock mode (can be closed from another terminal)\n");
    printf(" -o: Use old-style scrolling\n");
    printf(" -h: Print usage and exit\n");
    printf(" -n: No bold characters (overrides -b and -B, default)\n");
    printf(" -s: \"Screensaver\" mode, exits on first keystroke\n");
    printf(" -x: mtx.(pcf|otb) font mode (uses matrix console font encoding, assumes terminal is using it)\n");
    printf(" -V: Print version information and exit\n");
    printf(" -M [message]: Prints your message in the center of the screen. Overrides -L's default message.\n");
    printf(" -u delay (0 - 10, default 4): Screen update delay\n");
    printf(" -C [color]: Use this color for matrix (default green)\n");
    printf(" -r: rainbow mode\n");
    printf(" -k: Characters change while scrolling. (Works without -o opt.)\n");
    printf(" -t [tty]: Set tty to use\n");
    usage_charsets();
}

void version(void) {
    printf("cmatrix version %s (compiled %s, %s)\n",
        VERSION, __TIME__, __DATE__);
    printf("\tGitHub repository: https://github.com/abishekvashok/cmatrix\n");
}
