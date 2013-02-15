# Makefile for the fsync package
# Copyright 2008 Parliament Hill Computers Ltd, all rights reserved.
# Author: ADDW December 2008
# SCCS: @(#)Makefile	1.1 11/14/11 22:37:38

# The distribution version # comes from the SCCS id of the spec file.
# To do a test build run something like:
#	make VERSION=test dist
# If the spec file is out on delta, go:
#	make VERSION=1.1 dist

PACKAGE=fsync
PROGS=fsync
SOURCES=fsync.c
BUILDPROGS=
SUBDIRS=docs
CONFIGS=configure.in config.h.in
DOCS=README Contributors License Hacks

SPECFILE=$(PACKAGE).spec
VERSION := $(shell sed -ne '/^%define \+version \+/s///p' $(SPECFILE) )
DESTDIR=/usr/local
BINDIR=$(DESTDIR)/bin/
DOCDIR=$(DESTDIR)/share/doc/$(PACKAGE)-$(VERSION)
MAKEFILE=Makefile
BUILDFILES=$(SPECFILE) $(MAKEFILE)

# Default target is to make the programs - not install
progs:	$(PROGS)

# Create directories then copy in script and documentation:
install:	$(PROGS)
	install -d $(BINDIR) $(DOCDIR)
	install -p $(PROGS) $(BINDIR)
	install -p -m 644 $(DOCS) $(DOCDIR)
	for d in $(SUBDIRS); do make -C $$d install ; done

# Create a tar file for distribution
dist:	$(PACKAGE)-$(VERSION).tar.gz

# The tar file for distribution:
$(PACKAGE)-$(VERSION).tar.gz: $(DOCS) $(BUILDPROGS) $(BUILDFILES) $(SOURCES) $(CONFIGS)
	mkdir -p .BuildDist/$(PACKAGE)-$(VERSION)
	cp -al $^ .BuildDist/$(PACKAGE)-$(VERSION)
	for d in $(SUBDIRS); do make -C $$d VERSION=$(VERSION) copy-dist ; done
	cd .BuildDist && tar czf ../$@ $(PACKAGE)-$(VERSION)
	rm -rf .BuildDist/$(PACKAGE)-$(VERSION)

cleandist:
	rm -f $(PACKAGE)-$(VERSION).tar.gz

clean:	cleandist
	for d in $(SUBDIRS); do make -C $$d VERSION=$(VERSION) clean ; done

fsync:	fsync.c config.h

# end
