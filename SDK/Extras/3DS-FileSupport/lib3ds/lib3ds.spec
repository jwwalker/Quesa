# Note that this is NOT a relocatable package
%define ver      1.1.0
%define prefix   /usr
BuildRoot: /tmp/lib3ds-%{PACKAGE_VERSION}-root

Summary: The 3D Studio File Format Library
Name: lib3ds
Version: %ver
Release: 1
Copyright: LGPL
Group: Development/Libraries
Source: http://download.sourceforge.net/lib3ds/lib3ds-%{ver}.tar.gz
URL: http://lib3ds.sourceforge.net
Docdir: %{prefix}/share/doc

%description

Lib3ds is a free alternative to Autodesk's 3DS File Toolkit for handling
3DS files It's main goal is to simplify the creation of 3DS import and
export filters.

This project is not related in any form to Autodesk. The library is
based on unofficial information about the 3DS format found on the web.

This  program  is  distributed in  the  hope that it will  be useful,  but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

The official Lib3ds Homepage can be found under:
  http://lib3ds.sourceforge.net

%prep
%setup

%build
./configure --prefix=%{prefix}
make

%install
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS ChangeLog NEWS README COPYING TODO
%{prefix}/bin/*
%{prefix}/lib/*
%{prefix}/include/lib3ds/*
%{prefix}/share/aclocal/*
%{prefix}/man/man1/*

