# Change if you like
CC = gcc
BINDIR = /usr/local/bin
XFONTDIR = /usr/X11R6/lib/X11/fonts
MANDIR = /usr/local/man
# Uncomment this for Debian Linux
#CONSOLEFONTDIR = /usr/share/consolefonts
# Uncomment this for Red Hat (default)
CONSOLEFONTDIR = /usr/lib/kbd/consolefonts

all: cmatrix.c
	$(CC) -O2 -Wall -o cmatrix cmatrix.c -lncurses
	strip cmatrix
	@echo
	@echo  " Will install cmatrix into $(BINDIR)"
	@echo  " Will install X fonts into $(XFONTDIR)"
	@echo  " Will install man page into $(MANDIR)"
	@echo  " Will install console fonts into $(CONSOLEFONTDIR)"
	@echo  " If this is not okay, edit the Makefile before running make install."
	@echo
	@echo  " Run \"make install\" as root to install CMatrix binaries and fonts."
	@echo  " For X Window support, run "make install.x" as root in X Window"
	

clean:
	rm cmatrix

install.x:
	@if [ `id -u` != '0' ]; then \
		echo "Must be root to run make install.x"; \
	else \
		echo ;\
		echo "Copying mtx.pcf font..." ;\
		cp -f mtx.pcf $(XFONTDIR)/misc/; \
		echo "Updating up X window font settings..." ;\
		cd $(XFONTDIR)/misc ; mkfontdir; \
		cd $(XFONTDIR)/misc; \
		xset fp rehash ;\
		echo "***************************************************************" ;\
		echo "* All done.  To enable X window support, plese use the -x flag *" ;\
		echo "* and make sure your xterm or rxvt uses the mtx.pcf font       *" ;\
		echo "* (xterm -fn mtx or rxvt -font mtx)                            *" ;\
		echo "*                                                              *" ;\
		echo "***************************************************************" ;\
	fi

install:
	@if [ `id -u` != '0' ]; then \
		echo "Must be root to run make install"; \
	else \
		echo ;\
		echo "Installing binary..." ;\
		cp -f cmatrix $(BINDIR) ;\
		echo "Installing console font..." ;\
		cp -f matrix.fnt $(CONSOLEFONTDIR)/ ;\
		echo "Installing RH6 console font..." ;\
		cp -f matrix.psf.gz $(CONSOLEFONTDIR)/ ;\
		echo "Installing man page..." ;\
		cp -f cmatrix.1 $(MANDIR)/man1 ;\
		echo "***************************************************************" ;\
		echo "* All done.   Please run make install.x for X Window support. *" ;\
		echo "* If you are unable to get the console font working, please    *" ;\
		echo "* check that the \"setfont\" program is in your PATH variable.   *" ;\
		echo "***************************************************************" ;\
	fi
