Summary: Alltray allows to dock any program.
Name: alltray
Version: 0.67
Release: 1
Prefix: %{prefix}
License: GPL
Group: Applications/System
Source: http://prdownloads.sourceforge.net/alltray/%{name}-%{version}.tar.gz
URL: http://alltray.sourceforge.net/
Packager: Michael Krylov <m.krylov@mail.ru>
BuildRoot: /var/tmp/%{name}-%{version}
Requires: gtk2 >= 2.2.0
BuildRequires: gtk2-devel >= 2.2.0

%description
With AllTray you can dock any application with no native tray icon (like Evolution, Thunderbird, Terminals) into the system tray. A high-light feature is that a click on the "close" button will minimize back to system tray. It works well with Gnome, KDE, XFCE 4, Fluxbox and WindowMaker.

%prep
%setup -n %{name}-%{version}

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/%{name}
%{_datadir}/man/*
%{_libdir}/liballtray.*
%{_datadir}/applications/alltray.desktop
%{_datadir}/pixmaps/alltray.png
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README

%changelog
* Sat Dec 10 2005 Michael Krylov <m.krylov@mail.ru>
- Initial spec for alltray-0.65



