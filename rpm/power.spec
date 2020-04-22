Name:       power_consumption

Summary:    Power consumption statistics
Version:    0.1
Release:    1
License:    Appache License 2.0
Source:     %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  qt5-qttools
BuildRequires:  qt5-qttools-linguist

%description
Application for battery usage and app statiscis.

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
%make_build

%install
%make_install

%files
%defattr(-,root,root,-)
/etc/systemd/system/batstat.service
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop

%post
chmod 644 /etc/systemd/system/batstat.service
systemctl enable batstat
