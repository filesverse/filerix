Name:           filerix
Version:        1.0.0
Release:        1%{?dist}
Summary:        A high-performance file management library

License:        MIT
URL:            https://github.com/filesverse/filerix
Source0:        %{url}/archive/refs/tags/v%{version}.tar.gz

BuildRequires:  gcc, gcc-c++, meson, ninja-build, pkgconfig, systemd-devel, zlib-devel
Requires:       glibc, udev, zlib

%description
Filerix is a lightweight and high-performance file management library 
designed to provide essential file system operations.

%package devel
Summary:        Development files for Filerix
Requires:       %{name} = %{version}-%{release}

%description devel
The Filerix development package contains headers and pkg-config files needed 
to develop applications using the Filerix library.

%prep
%autosetup -n %{name}-%{version}

%build
meson setup build --prefix=%{_prefix}
meson compile -C build

%install
meson install -C build --destdir=%{buildroot}

%files
%license LICENSE
%doc README.md
%{_libdir}/libfilerix.so

%files devel
%{_includedir}/filerix/*
%{_libdir}/pkgconfig/filerix.pc

%changelog
* Fri Feb 14 2025 KingMaj0r <kingmaj0r@hotmail.com> - 1.0.0-1
- Initial release
