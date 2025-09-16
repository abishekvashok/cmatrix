bin_PROGRAMS = cmatrix
cmatrix_SOURCES = cmatrix.c

man_MANS = cmatrix.1

if MATRIX_FONTS
BUILD_FONTS = 1
else
BUILD_FONTS = 0
endif

EXTRA_DIST =	COPYING INSTALL install-sh \
		missing mkinstalldirs matrix.fnt \
		matrix.psf.gz mtx.pcf cmatrix.1 cmatrix.spec

install-data-local:
	@if test $(BUILD_FONTS) = 1; then \
		if test -d /usr/share/consolefonts; then \
	    		echo " Installing matrix fonts in /usr/share/consolefonts..."; \
	    		mkdir -p $(DESTDIR)/usr/share/consolefonts; \
	    		$(INSTALL_DATA) $(srcdir)/matrix.fnt $(DESTDIR)/usr/share/consolefonts; \
	    		$(INSTALL_DATA) $(srcdir)/matrix.psf.gz $(DESTDIR)/usr/share/consolefonts; \
		fi; \
		if test -d /usr/lib/kbd/consolefonts; then \
	    		echo " Installing matrix fonts in /usr/lib/kbd/consolefonts..."; \
	    		$(INSTALL_DATA) $(srcdir)/matrix.fnt $(DESTDIR)/usr/lib/kbd/consolefonts; \
	    		$(INSTALL_DATA) $(srcdir)/matrix.psf.gz $(DESTDIR)/usr/lib/kbd/consolefonts; \
		fi; \
		if test -d /usr/share/fonts/misc; then \
	    		echo " Installing X window matrix fonts in /usr/share/fonts/misc..."; \
	    		echo " Running mkfontdir /usr/share/fonts/misc..."; \
	    		$(MKFONTDIR) $(DESTDIR)/usr/share/fonts/misc; \
	    		$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/share/fonts/misc; \
	    		$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/share/fonts/misc; \
	    		echo " Done.  If this is the first time you have installed CMatrix you will"; \
	    		echo " probably have to restart X window in order to use the mtx.pcf font."; \
		elif test -d /usr/share/X11/fonts/misc; then \
	    		echo " Installing X window matrix fonts in /usr/share/X11/fonts/misc..."; \
	    		echo " Running mkfontdir /usr/share/X11/fonts/misc..."; \
	    		$(MKFONTDIR) $(DESTDIR)/usr/share/X11/fonts/misc; \
	    		$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/share/X11/fonts/misc; \
	    		$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/share/X11/fonts/misc; \
	    		echo " Done.  If this is the first time you have installed CMatrix you will"; \
	    		echo " probably have to restart X window in order to use the mtx.pcf font."; \
		else \
			if test -d /usr/X11R6/lib/X11/fonts/misc; then \
	    			echo " Installing X window matrix fonts in /usr/X11R6/lib/X11/fonts/misc..."; \
	    			echo " Running mkfontdir /usr/X11R6/lib/X11/fonts/misc..."; \
	    			$(MKFONTDIR) $(DESTDIR)/usr/X11R6/lib/X11/fonts/misc; \
	    			$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/X11R6/lib/X11/fonts/misc; \
	    			$(INSTALL_DATA) $(srcdir)/mtx.pcf $(DESTDIR)/usr/X11R6/lib/X11/fonts/misc; \
	    			echo " Done.  If this is the first time you have installed CMatrix you will"; \
	    			echo " probably have to restart X window in order to use the mtx.pcf font."; \
			fi \
		fi; \
	else \
		echo "Not installing matrix fonts"; \
	fi
