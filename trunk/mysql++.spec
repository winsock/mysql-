Summary: C++ wrapper for the MySQL C API
Name: mysql++
Version: 1.9.0
Release: 1
Copyright: LGPL
Group: Development/Databases
Requires: MySQL-shared
Source: http://tangentsoft.net/mysql++/releases/mysql++-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
%description
MySQL++ makes working with MySQL server queries as easy as working
with STL containers.  This package contains only the libraries needed to
run MySQL++-based programs.  If you are building your own MySQL++-based
programs, you also need to install the -devel package.

%package devel
Summary: MySQL++ developer files (headers, examples, etc.)
Group: Development/Databases
Requires: mysql++
%description devel
These are the files needed to compile MySQL++ based programs, plus
some sample code to get you started.  If you aren't building your own
programs, you probably don't need to install this package.

%package manuals
Summary: MySQL++ user and reference manuals
Group: Development/Databases
%description manuals
This is the MySQL++ documentation.  It's a separate RPM just because
it's so large, and it doesn't change with every release.

%prep
%setup -q
./configure --prefix=/usr --includedir=/usr/include/mysql++ --disable-examples

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/lib
mkdir -p $RPM_BUILD_ROOT/usr/include
mkdir -p $RPM_BUILD_ROOT/usr/src/mysql++/examples
%{__make} install DESTDIR=$RPM_BUILD_ROOT
install -m644 examples/*.cpp examples/*.h $RPM_BUILD_ROOT/usr/src/mysql++/examples
install -m644 examples/Makefile.simple $RPM_BUILD_ROOT/usr/src/mysql++/examples/Makefile
cp examples/README doc/README.examples
cp LGPL doc/LICENSE
rm $RPM_BUILD_ROOT/usr/lib/*.la
rm $RPM_BUILD_ROOT/usr/lib/*.so
rm $RPM_BUILD_ROOT/usr/lib/*.so.[0-9]

%clean
rm -rf $RPM_BUILD_ROOT
rm -f doc/README.examples doc/LICENSE

%post
	ln -sf /usr/lib/libmysqlpp.so.%{version} /usr/lib/libmysqlpp.so
	/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc doc/LICENSE doc/README.mysql++

/usr/lib/libmysqlpp.*

%files devel
%defattr(-,root,root)
%doc doc/README.devel doc/README.examples

/usr/include/mysql++
/usr/src/mysql++/examples

%files manuals
%defattr(-,root,root)
%doc doc/*.pdf doc/refman doc/userman

%changelog
* Fri Jun 24 2005 Warren Young <mysqlpp@etr-usa.com> 1.9.0-1
- Updated for internal-only 1.9.0 release

* Thu May 26 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.40-1
- Updated for 1.7.40

* Thu May 5 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.35-1
- Updated for 1.7.35

* Sat Apr 30 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.34-1
- Updated for 1.7.34
- Split manuals out into their own sub-package.

* Fri Apr 29 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.33-1
- Updated for 1.7.33

* Thu Mar 10 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.32-1
- Updated for 1.7.32
- Disabled building of examples, to speed RPM build.

* Fri Mar 5 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.31-1
- Updated for 1.7.31

* Mon Feb 28 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.30-1
- Updated for 1.7.30

* Fri Feb 4 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.28-1
- Updated for 1.7.28

* Wed Jan 12 2005 Warren Young <mysqlpp@etr-usa.com> 1.7.27-1
- Updated for 1.7.27

* Fri Dec 17 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.26-1
- Updated for 1.7.26

* Thu Dec 9 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.25-1
- Updated for 1.7.25

* Wed Dec 8 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.24-1
- Updated for 1.7.24

* Sat Nov 20 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.23-1
- Updated for 1.7.23

* Wed Nov 17 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.22-1
- Updated for 1.7.22

* Fri Nov 05 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.21-1
- Updated for 1.7.21
- Split out -devel subpackage, which now includes the examples

* Wed Nov 03 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.20-1
- Updated for 1.7.20

* Mon Oct 25 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.19-1
- Updated for 1.7.19

* Fri Oct 1 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.18-1
- Updated for 1.7.18

* Thu Sep 16 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.17-1
- Updated for 1.7.17

* Thu Sep 13 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.16-1
- Updated for 1.7.16

* Thu Sep 2 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.15-1
- Updated for 1.7.15

* Thu Aug 26 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.14-1
- Updated for 1.7.14

* Mon Aug 23 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.13-1
- Updated for 1.7.13

* Thu Aug 19 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.12-1
- Updated for 1.7.12

* Wed Aug 18 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.11-1
- Updated for 1.7.11
- Removed examples from documentation.
- Limited documentation to just the generated files, not the sources.

* Tue Aug 17 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.10-1
- Updated for 1.7.10

* Wed Apr 16 2003 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-4
- Added gcc 3.2.2 patch.
- Packaged using Red Hat Linux 8.0 and 9.

* Thu Nov 14 2002 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-3
- Changed the version passed to libtool.

* Mon Oct 28 2002 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-2
- Updated the version numbering of the library to be 1.7.9.
- Packaged using Red Hat Linux 8.0.

* Thu Oct 17 2002 Philipp Berndt <philipp.berndt@gmx.net>
- packaged
