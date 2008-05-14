#!/bin/sh
#
# @file pkg_install_fc4.sh
# @brief OpenRTM-aist dependent packages install script for Fedora Core 4
# @author Noriaki Ando <n-ando@aist.go.jp>
#
# $Id$
#

omniORB_spec () {
cat << EOF
# defined empty to enable automatic uid/gid selection.
# set values to select specific user/group ids.
%define omniuid -1
%define omnigid -1
%define _unpackaged_files_terminate_build      0

Summary: Object Request Broker (ORB)
Name:    omniORB
Version: 4.0.6
Release: 2
License: GPL / LGPL
Group:   System/Libraries
Source0: %{name}-%{version}.tar.gz
Prefix:  /usr
Prereq:  /sbin/ldconfig
URL:     http://omniorb.sourceforge.net/
#Provides:       corba
BuildRequires:  python glibc-devel
%if "%{_vendor}" == "MandrakeSoft"
BuildRequires:  openssl-devel
%endif
%if "%{_vendor}" == "redhat"
BuildRequires:  python-devel openssl-devel
%endif
%if "%{_vendor}" == "suse"
BuildRequires:	openssl-devel
%endif
Buildroot:      %{_tmppath}/%{name}-%{version}-root
#BuildArch:      i586

%description
%{name} is an Object Request Broker (ORB) which implements
specification 2.6 of the Common Object Request Broker Architecture
(CORBA). Contains the libraries needed to run programs dynamically
linked with %{name}.

# servers

%package servers
Summary: Utility programs
Group:          Development/C++
%if "%{_vendor}" == "suse"
Prereq:         /sbin/insserv
%else
Prereq:         /sbin/service /sbin/chkconfig
%endif
Prereq:         /usr/sbin/groupadd /usr/sbin/groupdel
Prereq:         /usr/sbin/useradd /usr/sbin/userdel
Requires:       %{name} = %{version}-%{release}
Provides:       libomniorb-servers = %{version}-%{release} %{name}-servers = %{version}-%{release}

%description servers
%{name} CORBA services including a Naming Service.

%package bootscripts
Summary: Utility programs
Group: Development/C++
Requires: %{name}-servers = %{version}-%{release} %{name}-utils = %{version}-%{release}
Provides: %{name}-bootscripts = %{version}-%{release}

%description bootscripts
Automatic starting of the %{name} CORBA Naming Service.

# utilities

%package utils
Summary: Utility programs
Group:          Development/C++
Requires:       %{name} = %{version}-%{release}
Provides:       libomniorb-utils = %{version}-%{release} %{name}-utils = %{version}-%{release}

%description utils
%{name} utility programs which may be useful at runtime.

# devel part of the bundle

%package devel
Summary: Header files and libraries needed for %{name} development
Group:          Development/C++
Requires:       %{name} = %{version}-%{release}
Provides:       libomniorb-devel = %{version}-%{release} %{name}-devel = %{version}-%{release}

%description devel
The header files and libraries needed for developing programs using
%{name}.

# docs and examples are in a separate package

%package doc
Summary: Documentation and examples for %{name}
Group:          Development/C++
#Requires:       %{name} = %{version}

%description doc
Developer documentation and examples.


%define py_ver    %(python -c 'import sys;print(sys.version[0:3])')


%prep
%setup -n %{name}-%{version}

%if "%{_vendor}" == "suse"
# Replace the init script with something appropriate for SUSE.
# Note that we hardcode a relative path here, since we are replacing
# a file in the source distribution tree.
cp -f etc/init.d/omniNames.SuSE.in etc/init.d/omniNames.in
%endif

./configure --prefix=%{prefix} --with-openssl=%{prefix}


%build
# We abuse the CPPFLAGS to pass optimisation options through.
make IMPORT_CPPFLAGS+="$RPM_OPT_FLAGS" all


%install
make DESTDIR=%{buildroot} install

mkdir -p %{buildroot}%{_initrddir}
mkdir -p %{buildroot}%{_sysconfdir}
cp sample.cfg %{buildroot}%{_sysconfdir}/omniORB.cfg
cp etc/init.d/omniNames %{buildroot}%{_initrddir}

mkdir -p %{buildroot}%{_mandir}/man{1,5}
cp -r man/* %{buildroot}%{_mandir}

mkdir -p %{buildroot}%{_var}/omniNames
mkdir -p %{buildroot}%{_localstatedir}/omniMapper

# Rename catior to avoid naming conflict with TAO
mv %{buildroot}%{_bindir}/catior %{buildroot}%{_bindir}/catior.omni
mv %{buildroot}%{_mandir}/man1/catior.1 %{buildroot}%{_mandir}/man1/catior.omni.1

%if "%{_vendor}" == "suse"
  # Most SUSE service scripts have a corresponding link into /usr/sbin
  mkdir -p %{buildroot}%{_sbindir}
  ln -sf %{_initrddir}/omniNames %{buildroot}%{_sbindir}/rcomniNames
%endif


%clean
[ -z %{buildroot} ] || rm -rf %{buildroot}


%pre

%post
/sbin/ldconfig

%pre servers
%if "%{omnigid}" == "-1"
OMNIGIDOPT="-r"
%else
OMNIGIDOPT="-g %{omnigid}"
%endif
%if "%{omniuid}" == "-1"
OMNIUIDOPT="-r"
%else
OMNIUIDOPT="-u %{omniuid}"
%endif
/usr/sbin/groupadd ${OMNIGIDOPT} omni >/dev/null 2>&1 || :
/usr/sbin/useradd ${OMNIUIDOPT} -M -g omni -d /var/omniNames \
  -s /bin/bash -c "omniORB servers" omni >/dev/null 2>&1 || :

%pre bootscripts
# A previous version is already installed?
if [ $1 -ge 2 ]; then
%if "%{_vendor}" == "suse"
  %{_sbindir}/rcomniNames stop >/dev/null 2>&1
%else
  /sbin/service omniNames stop >/dev/null 2>&1
%endif
fi

%post bootscripts
%if "%{_vendor}" == "suse"
/sbin/insserv omniNames
#%{_sbindir}/rcomniNames restart >/dev/null 2>&1
%else
/sbin/chkconfig --add omniNames
#/sbin/service omniNames restart >/dev/null 2>&1
%endif

%preun bootscripts
# Are we removing the package completely?
if [ $1 -eq 0 ]; then
%if "%{_vendor}" == "suse"
  %{_sbindir}/rcomniNames stop >/dev/null 2>&1
  /sbin/insserv -r omniNames
%else
  /sbin/service omniNames stop >/dev/null 2>&1
  /sbin/chkconfig --del omniNames
%endif
  rm -rf /var/omniNames/*
  rm -rf /var/lib/omniMapper/*
fi

%postun
/sbin/ldconfig

%postun servers
# uninstalling all versions?
if [ $1 -eq 0 ] ; then
  /usr/sbin/userdel omni >/dev/null 2>&1 || :
  /usr/sbin/groupdel omni >/dev/null 2>&1 || : 
fi


# main package includes libraries and copyright info
%files
%defattr (-,root,root)
%doc CREDITS COPYING COPYING.LIB
%config(noreplace) %{_sysconfdir}/*.cfg
%{_libdir}/*.so.*
%{_datadir}/idl/*


%files servers
%defattr (-,root,root)
%dir %attr(700,omni,omni) %{_var}/omniNames
%dir %attr(700,omni,omni) %{_localstatedir}/omniMapper
%attr(644,root,man) %{_mandir}/man1/omniNames*
#%attr(644,root,man) %{_mandir}/man1/omniMapper*
%attr(755,root,root) %{_bindir}/omniMapper
%attr(755,root,root) %{_bindir}/omniNames
# Thin substitute for standard Linux init script


%files bootscripts
%defattr (-,root,root)
%config(noreplace) %attr(775,root,root) %{_initrddir}/*
%if "%{_vendor}" == "suse"
%{_sbindir}/rcomniNames
%endif


%files utils
%defattr (-,root,root)
%attr(644,root,man) %{_mandir}/man1/catior*
%attr(644,root,man) %{_mandir}/man1/genior*
%attr(644,root,man) %{_mandir}/man1/nameclt*
%{_bindir}/catior.omni
%{_bindir}/convertior
%{_bindir}/genior
%{_bindir}/nameclt


%files devel
%defattr(-,root,root)
%doc ReleaseNotes* readmes/*
%attr(644,root,man) %{_mandir}/man1/omniidl*
%{_bindir}/omnicpp
%{_bindir}/omniidl
%{_bindir}/omniidlrun.py
%{_bindir}/omkdepend
%{_libdir}/*.a
%{_libdir}/*.so
%{_includedir}/*
%{_libdir}*/python%{py_ver}/site-packages/omniidl/*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/*.py*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/cxx/*.py*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/cxx/header/*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/cxx/skel/*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/cxx/dynskel/*
%{_libdir}*/python%{py_ver}/site-packages/omniidl_be/cxx/impl/*
%{_libdir}*/python%{py_ver}/site-packages/_omniidlmodule.so*
%{_libdir}/pkgconfig/*.pc


%files doc
%defattr(-,root,root)
%doc doc/* 


%changelog
* Thu Apr 21 2005 Sander Steffann <steffann@nederland.net> 4.0.6-2
- Fixed packaging issues for RHEL and x86_64

* Wed Apr 20 2005 Sander Steffann <steffann@nederland.net> 4.0.6-1
- Upgrade to version 4.0.6

* Mon Jul 26 2004 Duncan Grisby <duncan@grisby.org> 4.0.4-1
- Bump version number; integrate SUSE changes. Dont automatically
  start omniNames upon RPM install.

* Thu Jul 22 2004 Thomas Lockhart <lockhart@fourpalms.org> 4.0.3-7
- Incorporate additional SUSE features per Dirk O. Siebnich <dok@dok-net.net>
- Use additional standard RPM substitution parameters rather than
  hardcoded paths

* Wed Dec 24 2003 Thomas Lockhart <lockhart@fourpalms.org> 4.0.3
- Fix ownership of boot scripts per Bastiann Bakker
- Clean up pre- and post-install actions to support servers

* Tue Dec 08 2003 Thomas Lockhart <lockhart@fourpalms.org> 4.0.3
- Include additional build dependencies for redhat per Bastiann Bakker
- Put man pages for all distros into %{prefix}/share/man per FHS conventions
- Run omniNames under user "omni" per Jan Holst Jensen

* Mon Dec 01 2003 Thomas Lockhart <lockhart@fourpalms.org> 4.0.3
- Merge SuSE spec contributions from Johan Cronje

* Wed Nov 19 2003 Duncan Grisby <duncan@grisby.org> 4.0.3
- Merge contributed updates, bump version number.

* Fri Aug 08 2003 Thomas Lockhart <lockhart@fourpalms.org> 4.0.2
- Rename catior man page to match catior.omni binary name

* Wed Aug  6 2003 Duncan Grisby <dgrisby@apasphere.com> 4.0.2
- Bump version number.

* Tue Jun 10 2003 Duncan Grisby <dgrisby@apasphere.com> 4.0.2pre1
- Fix some text, bump version number, add init script, minor tweaks.

* Wed Feb 12 2003 Thomas Lockhart <lockhart@fourpalms.org> 4.0.0
- Rename catior to catior.omni to avoid name conflict with TAO

* Tue Oct 01 2002 Thomas Lockhart <lockhart@fourpalms.org> 4.0.0
- Track down changes in documentation for 4.0.0
- Omit patches required to build the previous beta

* Mon Jul 29 2002 Thomas Lockhart <lockhart@fourpalms.org> 4.0.0beta
- Separate out utility programs to manage name conflict for catior with TAO

* Wed Jul 03 2002 Thomas Lockhart <lockhart@fourpalms.org> 4.0.0beta
- Start from 3.04 spec files
- Strip workarounds from the spec file since 4.0 builds more cleanly


EOF
}

rpm_dir='/usr/src/redhat/RPMS/'`uname -i`

boost=`rpm -qa boost`
boost_devel=`rpm -qa boost-devel`

ace=`rpm -qa ace`
ace_devel=`rpm -qa ace-devel`

ace_pkg="http://dist.bonsai.com/ken/ace_tao_rpm/FC4.i386/5.4.10-1/ace-5.4.10-1.FC4.i386.rpm"
ace_devel_pkg="http://dist.bonsai.com/ken/ace_tao_rpm/FC4.i386/5.4.10-1/ace-devel-5.4.10-1.FC4.i386.rpm"


omniorb=`rpm -qa omniORB`
omniorbpy=`rpm -qa omniORBpy`

omniorb_pkg="http://opensource.nederland.net/omniORB/downloads/4.0.6/SRPMS/omniORB-4.0.6-2.src.rpm"
omniorbpy_pkg="http://opensource.nederland.net/omniORB/downloads/4.0.6/SRPMS/omniORBpy-2.6-2.src.rpm"
omniorb_src="ftp://ftp.jp.freebsd.org/pub/FreeBSD/ports/distfiles/omniORB-4.0.6.tar.gz"

if test "x$boost" = "x" ; then
	echo "boost is not installed."
	echo "Installing boost"
	yum install boost
	echo "done"
else
	echo "boost is already installed."
fi
 
if test "x$boost_devel" = "x" ; then
	echo "boost-devel is not installed."
	echo "Installing boost-devel"
	yum install boost-devel
	echo "done"
else
	echo "boost-devel is already installed."
fi
 
if test "x$ace" = "x" ; then
	echo "ace is not installed."
	echo "downloading ace...."
	wget $ace_pkg
	echo "Installing ace...."
	rpm -ivh ace-5.4.10-1.FC4.i386.rpm
	echo "done"
else
	echo "ace is already installed."
fi

if test "x$ace_devel" = "x" ; then
	echo "ace-devel is not installed."
	echo "downloading ace-devel...."
	wget $ace_devel_pkg
	echo "Installing ace-devel...."
	rpm -ivh ace-devel-5.4.10-1.FC4.i386.rpm
	echo "done"
else
	echo "ace-devel is already installed."
fi

if test "x$omniorb" = "x" ; then
	echo "omniORB is not installed."
	if ! [ -f $rpm_dir"/omniORB-4.0.6-2.i386.rpm" ] ; then
		echo "downloading omniORB...."
		mkdir -p /usr/src/redhat/SOURCES
		(cd /usr/src/redhat/SOURCES ; wget $omniorb_src )
		echo "Rebuilding omniORB...."
		omniORB_spec > omniORB.spec
		rpmbuild -ba omniORB.spec
	else
		echo "Pre-build package was found."
	fi
	echo "Installing omniORB...."
	( cd $rpm_dir ; rpm -ivh omniORB-4.0.6-2.i386.rpm )
	( cd $rpm_dir ; rpm -ivh omniORB-devel-4.0.6-2.i386.rpm )
	( cd $rpm_dir ; rpm -ivh omniORB-doc-4.0.6-2.i386.rpm )
	( cd $rpm_dir ; rpm -ivh omniORB-servers-4.0.6-2.i386.rpm )
	( cd $rpm_dir ; rpm -ivh omniORB-utils-4.0.6-2.i386.rpm )
	( cd $rpm_dir ; rpm -ivh omniORB-bootscripts-4.0.6-2.i386.rpm )
	echo "Making /var/omninames"
	mkdir -p /var/omninames
	echo "done"
else
	echo "omniORB is already installed."
fi

if test "x$omniorbpy" = "x" ; then
	echo "omniORBpy is not installed."

	echo "Chacking Orbit-py."
	pyorbit=`rpm -qa pyorbit`
	if ! test "x$pyorbit" = "x" ; then
		echo "ORBit-py is already installed."
		echo "omniORBpy files conflict with ORBit files (eg. CORBA.py)."
		echo "omniORBpy installation is stopped."
	else
		if ! [ -f $rpm_dir"/omniORBpy-2.6-2.i386.rpm" ] ; then
			echo "downloading omniORBpy...."
			wget $omniorbpy_pkg
			echo "Rebuilding omniORBpy...."
			rpmbuild --rebuild omniORBpy-2.6-2.src.rpm
		else
			echo "Pre-build package was found."
		fi
		echo "Installing ommniORBpy...."
		(cd $rpm_dir ; rpm -ivh omniORBpy-2.6-2.i386.rpm)
		(cd $rpm_dir ; rpm -ivh omniORBpy-devel-2.6-2.i386.rpm)
		(cd $rpm_dir ; rpm -ivh omniORBpy-doc-2.6-2.i386.rpm)
		(cd $rpm_dir ; rpm -ivh omniORBpy-standard-2.6-2.i386.rpm)
		echo "done"
	fi
else
	echo "omniORBpy is already installed."
fi
