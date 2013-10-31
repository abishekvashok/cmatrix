bin_PROGRAMS = cmatrix
cmatrix_SOURCES = cmatrix.c

man_MANS = cmatrix.1

EXTRA_DIST =	AUTHORS BUGS COPYING ChangeLog INSTALL NEWS README \
		TODO install-sh missing mkinstalldirs matrix.fnt \
		matrix.psf.gz mtx.pcf

install-data-local:
	@if test -d /usr/share/consolefonts; then \
	    echo " Installing matrix fonts in /usr/share/consolefonts..."; \
	    $(INSTALL_DATA) $(srcdir)/matrix.fnt /usr/share/consolefonts; \
	    $(INSTALL_DATA) $(srcdir)/matrix.psf.gz /usr/share/consolefonts; \
	fi
	@if test -d /usr/lib/kbd/consolefonts; then \
	    echo " Installing matrix fonts in /usr/lib/kbd/consolefonts..."; \
	    $(INSTALL_DATA) $(srcdir)/matrix.fnt /usr/lib/kbd/consolefonts; \
	    $(INSTALL_DATA) $(srcdir)/matrix.psf.gz /usr/lib/kbd/consolefonts; \
	fi
	@if test -d /usr/lib/X11/fonts/misc; then \
	    echo " Installing X window matrix fonts in /usr/lib/X11/fonts/misc..."; \
	    $(INSTALL_DATA) $(srcdir)/mtx.pcf /usr/lib/X11/fonts/misc; \
	    $(INSTALL_DATA) $(srcdir)/mtx.pcf /usr/lib/X11/fonts/misc; \
	    echo " Running mkfontdir /usr/lib/X11/fonts/misc..."; \
	    $(MKFONTDIR) /usr/lib/X11/fonts/misc; \
	    echo " Done.  If this is the first time you have installed CMatrix you will"; \
	    echo " probably have to restart X window in order to use the mtx.pcf font."; \
	else \
	if test -d /usr/X11R6/lib/X11/fonts/misc; then \
	    echo " Installing X window matrix fonts in /usr/X11R6/lib/X11/fonts/misc..."; \
	    $(INSTALL_DATA) $(srcdir)/mtx.pcf /usr/X11R6/lib/X11/fonts/misc; \
	    $(INSTALL_DATA) $(srcdir)/mtx.pcf /usr/X11R6/lib/X11/fonts/misc; \
	    echo " Running mkfontdir /usr/X11R6/lib/X11/fonts/misc..."; \
	    $(MKFONTDIR) /usr/X11R6/lib/X11/fonts/misc; \
	    echo " Done.  If this is the first time you have installed CMatrix you will"; \
	    echo " probably have to restart X window in order to use the mtx.pcf font."; \
	fi \
	fi
