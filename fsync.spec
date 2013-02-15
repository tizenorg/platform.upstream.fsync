# RPM specfile for the fsync package
# Copyright 2009 Parliament Hill Computers Ltd, all rights reserved.
# Author: ADDW September 2009
# SCCS: @(#)fsync.spec	1.1 11/14/11 22:48:57
# Note that the version number (1.1 - SCCS %-I-% (sans -)) of this file defines the package version #. It is used in the Makefile.

%define name fsync
%define version 1.1
%define release 1.1

Summary: synchronize file's in-core state with storage device
Name: %{name}
Version: %{version}
Release: %{release}
Source: http://www.phcomp.co.uk/Downloads/fsync-%{version}.tar.gz
Vendor: %{vendor}
URL: %{phcl_url}

License: GPL

Packager: %{packager}

Group: Applications/System
BuildRoot: %{_builddir}/%{name}-root
BuildRequires: make sed gcc autoconf

# This is only needed if we want to build a relocatable RPM:
#Prefix: %{_prefix}

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
autoconf
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT%{_prefix} VERSION=%{version} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_bindir}/fsync
%doc README Contributors License Hacks
%{_mandir}/man*/fsync.*

%changelog
* Mon Nov 11 2011 Alain Williams <addw@phcomp.co.uk>
- Write man page & other bits for proper release
* Thu Sep 10 2009 Alain Williams <addw@phcomp.co.uk>
- First time as an RPM

