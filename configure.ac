dnl Process this file with autoconf to produce a configure script.

AC_INIT([cmatrix], [2.0], [abishekvashok@gmail.com])
AC_CONFIG_SRCDIR([cmatrix.c])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AC_CANONICAL_HOST
case $host in
  *mingw*)
    ;;
  *)
    native_windows=no
    ;;
esac

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Checks for libraries.
dnl Replace `main' with a function in -lncurses:
AC_CHECK_LIB(ncurses, main)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h termios.h termio.h getopt.h)

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(putenv)

dnl Checks for libraries.
AC_CHECK_HEADERS(curses.h ncurses.h)

CURSES_LIB_NAME=""
AC_CHECK_LIB(ncurses, tgetent, CURSES_LIB="-lncurses" CURSES_LIB_NAME=ncurses)

if eval "test x$CURSES_LIB_NAME = x"
then
    AC_CHECK_LIB(curses, tgetent, CURSES_LIB="-lcurses" CURSES_LIB_NAME=curses)
fi

if eval "test x$CURSES_LIB_NAME = x"
then
    AC_CHECK_LIB(pdcurses, initscr, CURSES_LIB="-lpdcurses" CURSES_LIB_NAME=pdcurses)
fi

if eval "test x$CURSES_LIB_NAME = x"
then
    AC_CHECK_LIB(termcap, tgetent, CURSES_LIB="-ltermcap" CURSES_LIB_NAME=termcap)
fi

if eval "test x$CURSES_LIB_NAME = x"
then
    AC_CHECK_LIB(tinfo, tgetent, CURSES_LIB="-ltinfo" CURSES_LIB_NAME=tinfo)
fi

if eval "test x$CURSES_LIB_NAME = x"
then
    AC_MSG_WARN([
*** No termcap lib available, consider getting the official ncurses
*** distribution from ftp://ftp.gnu.org/pub/gnu/ncurses if you get
*** errors compiling cmatrix.])
else
    AC_MSG_RESULT("Using $CURSES_LIB_NAME as the termcap library")
fi

AC_CHECK_LIB($CURSES_LIB_NAME, use_default_colors,
	 AC_DEFINE(HAVE_USE_DEFAULT_COLORS))
AC_CHECK_LIB($CURSES_LIB_NAME, resizeterm, [AC_DEFINE(HAVE_RESIZETERM)])
AC_CHECK_LIB([$CURSES_LIB_NAME], wresize, [AC_DEFINE(HAVE_WRESIZE)])


dnl Only change gcc options if we are using gcc.
if test "$ac_cv_prog_CC" = gcc -o "$ac_cv_prog_CC" = g++; then
  CFLAGS="$CFLAGS -Wall -Wno-comment"
fi

dnl Build static executable on native Windows
if test x$native_windows != xno; then
  CFLAGS="$CFLAGS -static"
fi

dnl cmatrix terminal font disable option (default enabled)
AC_ARG_WITH([fonts],
	AS_HELP_STRING([--without-fonts], [Install cmatrix without cmatrix font]),
	AS_IF([test "x$with_fonts" == "xno"], [enable_fonts=false], [enable_fonts=true]),
	[enable_fonts=true])

if test "x$enable_fonts" != xfalse; then
  dnl Check for consolechars and setfonts
  AC_PATH_PROG(CONSOLECHARS, consolechars, "", $PATH:/usr/bin:/usr/local/bin/sbin:/usr/sbin)
  AC_PATH_PROG(SETFONT, setfont, "", $PATH:/usr/bin:/usr/local/bin/sbin:/usr/sbin)

  if test x$CONSOLECHARS = x; then
      if test x$SETFONT = x; then
	  AC_MSG_WARN([

*** neither the consolechars nor the setfont program was not found.  You
*** will not be able to see the characters in the matrix font in the
*** console without this program (it may still work in xterms). If you are
*** using Linux, the package containing this program is usually called
*** kbd, kbd-utils, or console-utils
])
      else
	  AC_DEFINE_UNQUOTED(HAVE_SETFONT, $SETFONT)
      fi
  else
      AC_DEFINE_UNQUOTED(HAVE_CONSOLECHARS, $CONSOLECHARS)
  fi

  if test x$CONSOLECHARS != x -o x$CONSOLECHARS != x'"$CONSOLECHARS"'; then
       if test x$SETFONT != x -o x$SETFONT != x'"$SETFONT"'; then

	  dnl Now look for the console fonts directory
	  AC_CHECK_FILES([/usr/lib/kbd/consolefonts /usr/share/consolefonts])

	  if test "x$ac_cv_file__usr_lib_kbd_consolefonts" = "xno"; then
	      if test "x$ac_cv_file__usr_share_consolefonts" = "xno"; then
		  AC_MSG_WARN([

*** You do not appear to have a consolefonts directory in a standard location
*** (/usr/lib/kbd or /usr/share), even though you appear to have the
*** consolechars and/or setfont command.  The matrix font for the console
*** will not be installed.  This means you will not be able to use the
*** matrix console font (and the -l command line switch) unless the font
*** is located in your current directory when you run CMatrix.
])
	      fi
	  fi
       fi
  fi

  AC_PATH_PROG(MKFONTDIR, mkfontdir, "", $PATH:/usr/bin:/usr/bin/X11:/usr/local/bin/X11:/usr/X11R6/bin:/usr/local/bin:/sbin:/usr/sbin)
  AC_CHECK_FILES([/usr/share/fonts/misc /usr/share/X11/fonts/misc /usr/X11R6/lib/X11/fonts/misc])

  if test "x$ac_cv_file__usr_lib_X11_fonts_misc" = "xno"; then
      if test "x$ac_cv_file__usr_X11R6_lib_X11_fonts_misc" = "xno"; then
	  AC_MSG_WARN([

*** You do not appear to have an X window fonts directory in the standard
*** locations (/usr/share/fonts/misc or /usr/X11R6/lib/X11/fonts/misc). The
*** mtx.pcf font will not be installed.  This means you will probably not
*** be able to use the mtx fonts in your x terminals, and hence be unable
*** to use the -x command line switch.  Sorry about that...
])
	  fi
      fi
fi

dnl Parse any configure options

LIBS="$LIBS $CURSES_LIB"
AC_ARG_ENABLE(debug, [  --enable-debug          Enable debugging (def disabled)],)
AM_CONDITIONAL([MATRIX_FONTS], [test x$enable_fonts = xtrue])

AC_SUBST(CURSES_LIB)

if test x$native_windows = xno; then
  AC_DEFINE(USE_TIOCSTI)
fi

AH_TEMPLATE([HAVE_USE_DEFAULT_COLORS], [Define this if your curses library has use_default_colors, for cool transparency =-)])
AH_TEMPLATE([HAVE_CONSOLECHARS], [Define this if you have the linux consolechars program])
AH_TEMPLATE([HAVE_SETFONT], [Define this if you have the linux setfont program])
AH_TEMPLATE([HAVE_WRESIZE], [Define this if you have the wresize function in your ncurses-type library])
AH_TEMPLATE([HAVE_RESIZETERM], [Define this if you have the resizeterm function in your ncurses-type library])
AH_TEMPLATE([USE_TIOCSTI], [Define this if you want a character you pressed in the screensaver mode to retain in the terminal])

AC_OUTPUT(Makefile cmatrix.spec)
