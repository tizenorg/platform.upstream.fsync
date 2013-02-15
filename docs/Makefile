# Makefile for the fsync package
# Copyright 2011 Parliament Hill Computers Ltd, all rights reserved.
# Author: ADDW November 2011
# SCCS: @(#)Makefile	1.1 11/14/11 22:30:30

# Makefile for docs directory

MANFILES=fsync.1
PACKAGE=fsync
MANDIR=$(DESTDIR)/share/man/man1

DISTDIR=../.BuildDist/$(PACKAGE)-$(VERSION)/docs

all:
	:

clean:
	:

install:
	mkdir -p $(MANDIR)
	install -m 644 $(MANFILES) $(MANDIR)

copy-dist: Makefile $(MANFILES)
	mkdir $(DISTDIR)
	cp -al $^ $(DISTDIR)

# end
