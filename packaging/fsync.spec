# RPM specfile for the fsync package
# Copyright 2009 Parliament Hill Computers Ltd, all rights reserved.
# Author: ADDW September 2009
# SCCS: @(#)fsync.spec	1.1 11/14/11 22:48:57
# Note that the version number (1.1 - SCCS %-I-% (sans -)) of this file defines the package version #. It is used in the Makefile.

Summary: synchronize file's in-core state with storage device
Name: fsync
Version: 1.1
Release: 1
Source: http://www.phcomp.co.uk/Downloads/fsync-%{version}.tar.gz
Source1001: %{name}.manifest
License: GPL
Group: Applications/System
BuildRequires: autoconf

%description
If system service is interrupted at the wrong moment (eg crash or power fail)
what is in the filesystem on reboot might inconsistent. This is especially
true with files in the process of being updated; eg where a modified copy of a
file is created that is then renamed over the original.
Flushing blocks to disk can help to avoid problems.
This is a command line interface to the fsync(2) and fdatasync(2) system calls.

%prep
%setup -q

%build
cp %{SOURCE1001} .
autoconf
%configure
make

%install
make DESTDIR=$RPM_BUILD_ROOT%{_prefix} VERSION=%{version} install

%remove_docs
%files
%defattr(-,root,root)
%manifest %{name}.manifest
%{_bindir}/fsync
%doc License

