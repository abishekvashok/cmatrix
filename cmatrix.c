/*
    cmatrix.c

    Copyright (C) 1999-2017 Chris Allegretta
    Copyright (C) 2017-Present Abishek V Ashok
    Copyright (c) 2019-2021 Fredrick R. Brennan <copypaste@kittens.ph>

    This file is part of cmatrix.

    cmatrix is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cmatrix is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cmatrix. If not, see <http://www.gnu.org/licenses/>.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <locale.h>

#include "cmatrix.h"
#include "util.h"
#include "charset.h"

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

static const char* LOCKED_MSG = _("Computer locked.");
static const char* LOCKEDNODIE_MSG = _("Cannot die, locked.");
static const char* LOCKEDKILL_MSG = _("Killed while locked!");
static const char* NORESIZE_MSG = _("Cannot resize window!");
static const char* NOTTYFONTSET_MSG = _("Unable to use both \"setfont\" and \"consolechars\".");
static const char* INVALIDCOLOR_MSG = _("Invalid color selection!\n"
                                      "Valid colors are green, red, blue, "
                                      "white, yellow, cyan, magenta " "and black.\n");
static const char* INVALIDUPDATE_MSG = _("Update speed invalid");
static const char* NOCHARSET_MSG = _("Invalid/no charset!");

/* Matrix typedef */
typedef struct cmatrix_cell {
    int val;
    bool is_head;
} cmatrix_cell;

/* Global variables */
bool console = false;
bool xwindow = false;
bool locked = false;
cmatrix_cell **matrix = NULL;
int *length = NULL;  /* Length of cols in each line */
int *spaces = NULL;  /* Spaces left to fill */
int *updates = NULL; /* What does this do again? */
size_t frame = 1;
static char **user_charset = NULL;
static size_t charset_len = 0;
const int char_offset = 0xff;
#ifndef _WIN32
volatile sig_atomic_t signal_status = 0; /* Indicates a caught signal */
size_t frame_signaled = 0;
#endif

/* What we do when we're all set to exit */
void cleanup() {
    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
    if (console) {
#ifdef HAVE_CONSOLECHARS
        va_system("consolechars -d");
#elif defined(HAVE_SETFONT)
        va_system("setfont");
#endif
    }
}

void finish() {
    cleanup();
    exit(EXIT_SUCCESS);
}

/* Initialize the global variables */
void var_init() {
    int i, j;

    if (matrix != NULL) {
        free(matrix[0]);
        free(matrix);
    }

    matrix = nmalloc(sizeof(cmatrix_cell *) * (LINES + 1));
    matrix[0] = nmalloc(sizeof(cmatrix_cell) * (LINES + 1) * COLS);
    for (i = 1; i <= LINES; i++) {
        matrix[i] = matrix[i - 1] + COLS;
    }

    if (length != NULL) {
        free(length);
    }
    length = nmalloc(COLS * sizeof(int));

    if (spaces != NULL) {
        free(spaces);
    }
    spaces = nmalloc(COLS* sizeof(int));

    if (updates != NULL) {
        free(updates);
    }
    updates = nmalloc(COLS * sizeof(int));

    /* Make the matrix */
    for (i = 0; i <= LINES; i++) {
        for (j = 0; j <= COLS - 1; j += 2) {
            matrix[i][j].val = -1;
        }
    }

    for (j = 0; j <= COLS - 1; j += 2) {
        /* Set up spaces[] array of how many spaces to skip */
        spaces[j] = (int) rand() % LINES + 1;

        /* And length of the stream */
        length[j] = (int) rand() % (LINES - 3) + 3;

        /* Sentinel value for creation of new objects */
        matrix[1][j].val = ' ';

        /* And set updates[] array for update speed. */
        updates[j] = (int) rand() % 3 + 1;
    }
}

#ifndef _WIN32
void sighandler(int s) {
    signal_status = s;
    frame_signaled = frame;
}
bool clearsignal() {
    if (signal_status > 0 && frame > frame_signaled + 10) {
        signal_status = 0;
        return true;
    }
    return false;
}
#endif

static const short LIGHTER_COLOR_OFFSET_INDEX = 8;
void sort_out_colors() {
    if (has_colors()) {
        start_color();
        /* Add in colors, if available */
#ifdef HAVE_USE_DEFAULT_COLORS
        if (use_default_colors() != ERR) {
            for (int i = 0; i < 255; i++) { 
                init_pair(i, i, -1);
            }
        } else {
#endif
            for (int i = 0; i < 255; i++) { 
                init_pair(i, i, COLOR_BLACK);
            }
#ifdef HAVE_USE_DEFAULT_COLORS
	}
#endif
        init_pair(255, COLOR_BLACK, COLOR_WHITE);
    }
}

void resize_screen(void) {
#ifdef _WIN32
    BOOL result;
    HANDLE hStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdHandle == INVALID_HANDLE_VALUE)
        return;
#else
    char *tty;
    int fd = 0;
    int result = 0;
    struct winsize win;

    tty = ttyname(0);
    if (!tty) {
        return;
#endif
#ifdef _WIN32
    result = GetConsoleScreenBufferInfo(hStdHandle, &csbiInfo);
    if (!result)
        return;
    LINES = csbiInfo.dwSize.Y;
    COLS = csbiInfo.dwSize.X;
#else
    }
    fd = open(tty, O_RDWR);
    if (fd == -1) {
        return;
    }
    result = ioctl(fd, TIOCGWINSZ, &win);
    if (result == -1) {
        return;
    }

    COLS = win.ws_col;
    LINES = win.ws_row;
#endif

    if (LINES < 10) {
        LINES = 10;
    }
    if (COLS < 10) {
        COLS = 10;
    }

#ifdef HAVE_RESIZETERM
    resizeterm(LINES, COLS);
#ifdef HAVE_WRESIZE
    if (wresize(stdscr, LINES, COLS) == ERR) {
        c_die(NORESIZE_MSG);
    }
#endif /* HAVE_WRESIZE */
#endif /* HAVE_RESIZETERM */

    var_init();
    /* Do these because width may have changed... */
    clear();
    refresh();
}

void change_charset(char optchr) {
    enum CHARSET optchrset;
    switch (optchr) {
        case 'A':
            optchrset = CHARSET_ASCII; break;
        case 'c':
            optchrset = CHARSET_KATAKANA; break;
        case 'm':
            optchrset = CHARSET_LAMBDA; break;
        case 'S':
            optchrset = CHARSET_SYMBOLS; break;
        case 'x':
            optchrset = CHARSET_FONT; break;
    }
    if (optchr == 'U') {
        if (optarg[0] == '\0') c_die(NOCHARSET_MSG);
        user_charset = expand_charset(optarg);
    }
    else if (optchr == 'm') user_charset = expand_charset(strdup("λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ λ"));
    else user_charset = expand_charset(charset_to_string(CHARSETS[optchrset]));
    charset_len = 0;
    for (char** ptr = user_charset; *ptr != NULL && **ptr != '\0'; ptr++) {
        charset_len++;
    }
    var_init();
    clear();
    refresh();
}

int main(int argc, char *argv[]) {
    char optchr, keypress;
    int count = 0;
    unsigned short update = 4;
    int mcolor = COLOR_GREEN;

    char *msg = "";
    char *tty = NULL;

    enum BOLD_STATE bold = BOLD_RANDOM;
    bool screensaver = false;
    bool asynch = false;
    bool force = false;
    bool oldstyle = false;
    bool rainbow = false;
    bool lambda = false;
    bool pause = false;
    bool classic = false;
    bool changes = false;
    bool inverted_heading = false;

    srand((unsigned) time(NULL));
    setlocale(LC_ALL, "");
#ifdef HAVE_LIBINTL_H
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
#endif

    while ((optchr = getopt(argc, argv, "aAbBcfhilLnrosSmxkVM:u:U:C:t:")) != EOF) {
        switch (optchr) {
        case 's':
            screensaver = true;
            break;
        case 'a':
            asynch = true;
            break;
        case 'b':
            if (bold != ALL_BOLD) {
                bold = BOLD_RANDOM;
            }
            break;
        case 'B':
            bold = ALL_BOLD;
            break;
        case 'C':
            if (!strcasecmp(optarg, "green")) {
                mcolor = COLOR_GREEN;
            } else if (!strcasecmp(optarg, "red")) {
                mcolor = COLOR_RED;
            } else if (!strcasecmp(optarg, "blue")) {
                mcolor = COLOR_BLUE;
            } else if (!strcasecmp(optarg, "white")) {
                mcolor = COLOR_WHITE;
            } else if (!strcasecmp(optarg, "yellow")) {
                mcolor = COLOR_YELLOW;
            } else if (!strcasecmp(optarg, "cyan")) {
                mcolor = COLOR_CYAN;
            } else if (!strcasecmp(optarg, "magenta")) {
                mcolor = COLOR_MAGENTA;
            } else if (!strcasecmp(optarg, "black")) {
                mcolor = COLOR_BLACK;
            } else {
                c_die(INVALIDCOLOR_MSG);
            }
            break;
        case 'f':
            force = true;
            break;
        case 'i':
            inverted_heading = true;
            break;
        case 'l':
            console = true;
            break;
        case 'L':
            locked = true;
            //if -M was used earlier, don't override it
            if (msg[0] == '\0') {
                msg = (char*)_(LOCKED_MSG);
            }
            break;
        case 'M':
            msg = strdup(optarg);
            break;
        case 'n':
            bold = NO_BOLD;
            break;
        case 'o':
            oldstyle = true;
            break;
        case 'u':
            char* next;
            update = strtol(optarg, &next, 10);
            if (*next != '\0') c_die(INVALIDUPDATE_MSG);
            update = min(update, 10);
            update = max(update, 0);
            break;
        case 'V':
            version();
            exit(EXIT_SUCCESS);
        case 'r':
            rainbow = true;
            break;
        case 'k':
            changes = true;
            break;
        case 't':
            tty = optarg;
            break;
        case 'A':
        case 'c':
        case 'm':
        case 'S':
        case 'U':
        case 'x':
            change_charset(optchr);
            break;
        case 'h': // fall through
        case '?':
        default:
            usage();
            exit(EINVAL);
        }
    }

    /* Clear TERM variable on Windows */
#ifdef _WIN32
    _putenv_s("TERM", "");
#endif

    if (force && strcmp("linux", getenv("TERM"))) {
#ifdef _WIN32
        SetEnvironmentVariableW(L"TERM", L"linux");
#else
        /* setenv is much more safe to use than putenv */
        //setenv("TERM", "linux", 1);
#endif
    }
    if (tty) {
        FILE *ftty = fopen(tty, "r+");
        if (!ftty) {
            fprintf(stderr, "cmatrix: error: '%s' couldn't be opened: %s.\n",
                    tty, strerror(errno));
            exit(EXIT_FAILURE);
        }
        SCREEN *ttyscr;
        ttyscr = newterm(NULL, ftty, ftty);
        if (ttyscr == NULL)
            exit(EXIT_FAILURE);
        set_term(ttyscr);
    } else {
        initscr();
    }
    savetty();
    nonl();
#ifdef _WIN32
    raw();
#else
    cbreak();
#endif
    noecho();
    timeout(0);
    leaveok(stdscr, TRUE);
    curs_set(0);
#ifndef _WIN32
    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGWINCH, sighandler);
    signal(SIGTSTP, sighandler);
#endif

    if (console) {
#ifdef HAVE_CONSOLECHARS
        cmd_or_die("consolechars", "consolechars -f matrix");
#elif defined(HAVE_SETFONT)
        cmd_or_die("setfont", "setfont matrix");
#else
        c_die(NOTTYFONTSET_MSG);
#endif
    }

    sort_out_colors();

    var_init();

    if (user_charset == NULL) change_charset('A');

    while (1) {
#ifndef _WIN32
        bool cleared = clearsignal();
        /* Check for signals */
        if (signal_status == SIGWINCH) {
            resize_screen();
            signal_status = 0;
        }

        if (!locked) {
            if (signal_status == SIGINT || signal_status == SIGQUIT) {
                finish();
                /* exits */
            }
    
            if (signal_status == SIGTSTP) {
                finish();
            }
        } else {
            if (signal_status == SIGINT || signal_status == SIGQUIT) {
                attron(COLOR_PAIR(COLOR_YELLOW));
                mvaddstr(1, 1, LOCKEDNODIE_MSG);
                attroff(COLOR_PAIR(COLOR_YELLOW));
            } else if (signal_status == SIGTERM) {
                c_die(LOCKEDKILL_MSG);
            } else if (cleared) {
                for (int ci = 0; ci < strlen(LOCKEDKILL_MSG); ci++) {
                    mvaddch(1, 1+ci, ' ');
                }
            }
        }
#endif

        count++;
        if (count > 4) {
            count = 1;
        }

        if ((keypress = wgetch(stdscr)) != ERR) {
            if (screensaver) {
#ifdef USE_TIOCSTI
                char *str = malloc(0);
                size_t str_len = 0;
                do {
                    str = realloc(str, str_len + 1);
                    str[str_len++] = keypress;
                } while ((keypress = wgetch(stdscr)) != ERR);
                for (size_t i = 0; i < str_len; i++)
                    ioctl(STDIN_FILENO, TIOCSTI, (char*)(str + i));
                free(str);
#endif
                finish();
            } else {
                switch (keypress) {
#ifdef _WIN32
                case 3: /* Ctrl-C. Fall through */
#endif
                case 'q':
                    if (!locked)
                        finish();
                    break;
                case 'a':
                    asynch = !asynch;
                    break;
                case 'b':
                    bold = BOLD_RANDOM;
                    break;
                case 'B':
                    bold = ALL_BOLD;
                    break;
                case 'i':
                    inverted_heading = !inverted_heading;
                    break;
                case 'L':
                    locked = true;
                    msg = (char*)LOCKED_MSG;
                    break;
                case 'n':
                    bold = NO_BOLD;
                    break;
                case '0': /* Fall through */
                case '1': /* Fall through */
                case '2': /* Fall through */
                case '3': /* Fall through */
                case '4': /* Fall through */
                case '5': /* Fall through */
                case '6': /* Fall through */
                case '7': /* Fall through */
                case '8': /* Fall through */
                case '9':
                    update = keypress - '0';
                    break;
                case '!':
                    mcolor = COLOR_RED;
                    rainbow = 0;
                    break;
                case '@':
                    mcolor = COLOR_GREEN;
                    rainbow = 0;
                    break;
                case '#':
                    mcolor = COLOR_YELLOW;
                    rainbow = 0;
                    break;
                case '$':
                    mcolor = COLOR_BLUE;
                    rainbow = 0;
                    break;
                case '%':
                    mcolor = COLOR_MAGENTA;
                    rainbow = 0;
                    break;
                case 'r':
                    rainbow = 1;
                    break;
                case 'A': /* Fall through */
                case 'c': /* Fall through */
                case 'm': /* Fall through */
                case 'S':
                case 'x':
                    change_charset(keypress);
                    break;
                case '^':
                    mcolor = COLOR_CYAN;
                    rainbow = 0;
                    break;
                case '&':
                    mcolor = COLOR_WHITE;
                    rainbow = 0;
                    break;
                case 'p':
                case 'P':
                    pause = !pause;
                    break;
                }
            }
        }

        int random;

        for (int j = 0; j <= COLS - 1; j += 2) {
            if ((count > updates[j] || !asynch) && !pause) {
                /* I don't like old-style scrolling, yuck */
                if (oldstyle) {
                    for (int i = LINES - 1; i >= 1; i--) {
                        matrix[i][j].val = matrix[i - 1][j].val;
                    }
                    random = (int) rand() % (charset_len + 8);

                    if (matrix[1][j].val == 0) {
                        matrix[0][j].val = 1;
                    } else if (matrix[1][j].val == ' '
                             || matrix[1][j].val == -1) {
                        if (spaces[j] > 0) {
                            matrix[0][j].val = ' ';
                            spaces[j]--;
                        } else {

                            /* Random number to determine whether head of next column
                               of chars has a white 'head' on it. */

                            if (((int) rand() % 3) == 1) {
                                matrix[0][j].val = 0;
                            } else {
                                matrix[0][j].val = (int) rand() % charset_len + char_offset;
                            }
                            spaces[j] = (int) rand() % LINES + 1;
                        }
                    } else if (random > charset_len && matrix[1][j].val != 1) {
                        matrix[0][j].val = ' ';
                    } else {
                        matrix[0][j].val = (int) rand() % charset_len + char_offset;
                    }
                } else { /* New style scrolling (default) */
                    if (matrix[0][j].val == -1 && matrix[1][j].val == ' ' && spaces[j] > 0) {
                        spaces[j]--;
                    } else if (matrix[0][j].val == -1 && matrix[1][j].val == ' ') {
                        length[j] = (int) rand() % (LINES - 3) + 3;
                        matrix[0][j].val = (int) rand() % charset_len + char_offset;

                        spaces[j] = (int) rand() % LINES + 1;
                    }
                    int i = 0;
                    int y = 0;
                    bool firstcoldone = false;
                    while (i <= LINES) {
                        /* Skip over spaces */
                        while (i <= LINES && (matrix[i][j].val == ' ' || matrix[i][j].val == -1)) {
                            i++;
                        }

                        if (i > LINES) {
                            break;
                        }

                        /* Go to the head of this column */
                        int z = i;
                        int y = 0;
                        while (i <= LINES && (matrix[i][j].val != ' ' && matrix[i][j].val != -1)) {
                            matrix[i][j].is_head = false;
                            if (changes) {
                                if (rand() % 8 == 0)
                                    matrix[i][j].val = (int) rand() % charset_len + char_offset;
                            }
                            i++;
                            y++;
                        }

                        if (i > LINES) {
                            matrix[z][j].val = ' ';
                            continue;
                        }

                        matrix[i][j].val = (int) rand() % charset_len + char_offset;
                        matrix[i][j].is_head = true;

                        /* If we're at the top of the column and it's reached its
                           full length (about to start moving down), we do this
                           to get it moving.  This is also how we keep segments not
                           already growing from growing accidentally =>
                         */
                        if (y > length[j] || firstcoldone) {
                            matrix[z][j].val = ' ';
                            matrix[0][j].val = -1;
                        }
                        firstcoldone = 1;
                        i++;
                    }
                }
            }
            /* A simple hack */
            int y, z;
            if (!oldstyle) {
                y = 1;
                z = LINES;
            } else {
                y = 0;
                z = LINES - 1;
            }
            for (int i = y; i <= z; i++) {
                move(i - y, j);
                char* stp = NULL;

                if (matrix[i][j].val > -1 && (matrix[i][j].val - char_offset) < charset_len && matrix[i][j].val != ' ') {
                    stp = user_charset[matrix[i][j].val - char_offset];
                } else {
                    stp = " ";
                }
                
                if (matrix[i][j].is_head) {
                    attron(COLOR_PAIR(inverted_heading?255:COLOR_WHITE));
                    if (bold >= BOLD_HEADING) {
                        attron(A_BOLD);
                    }

                    addstr(stp);

                    attroff(COLOR_PAIR(COLOR_WHITE));
                    if (bold) {
                        attroff(A_BOLD);
                    }
                } else {
                    if (rainbow) {
                        int randomColor = (matrix[i][j].val) % 6;

                        switch (randomColor) {
                            case 0:
                                mcolor = COLOR_GREEN;
                                break;
                            case 1:
                                mcolor = COLOR_BLUE;
                                break;
                            case 2:
                                mcolor = COLOR_RED;
                                break;
                            case 3:
                                mcolor = COLOR_YELLOW;
                                break;
                            case 4:
                                mcolor = COLOR_CYAN;
                                break;
                            case 5:
                                mcolor = COLOR_MAGENTA;
                                break;
                       }
                    }

                    bool onbold = (bold == ALL_BOLD || (bold == BOLD_RANDOM && matrix[i][j].val % 2 == 0));
                    bool oncolor = HAVE_256COLOR && (matrix[i][j].val % 2 == 0);
                    if (onbold) {
                        attron(A_BOLD);
                    }
                    if (oncolor) {
                        attron(COLOR_PAIR(LIGHTER_COLOR_OFFSET_INDEX+mcolor));
                    } else {
                        attron(COLOR_PAIR(mcolor));
                    }

                    addstr(stp);

                    if (onbold) {
                        attroff(A_BOLD);
                    }
                    if (oncolor) {
                        attroff(COLOR_PAIR(LIGHTER_COLOR_OFFSET_INDEX+mcolor));
                    } else {
                        attroff(COLOR_PAIR(mcolor));
                    }
                }
            }
        }

        //check if -M and/or -L was used
        if (msg[0] != '\0') {
            //Add our message to the screen
            int msg_x = LINES/2;
            int msg_y = COLS/2 - strlen(msg)/2;
            int i = 0;

            //Add space before message
            move(msg_x-1, msg_y-2);
            for (i = 0; i < strlen(msg)+4; i++)
                addch(' ');

            //Write message
            move(msg_x, msg_y-2);
            addstr("  ");
            addstr(msg);
            addstr("  ");

            //Add space after message
            move(msg_x+1, msg_y-2);
            for (i = 0; i < strlen(msg)+4; i++)
                addch(' ');
        }

        frame++;
        napms(update * 10);
    }
    finish();
}
