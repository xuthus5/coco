Name:           coco-uploader
Version:        0.0.1
Release:        1%{?dist}
Summary:        simple upload plugin.

License:        MIT
URL:            https://github.com/xuthus5/coco
Source0:        https://github.com/xuthus5/coco/archive/refs/tags/latest.tar.gz

BuildRequires:  meson
BuildRequires:  gtk4-devel
BuildRequires:  curl-devel
BuildRequires:  libadwaita-devel
BuildRequires:  gcc

%description

%prep
%autosetup -n coco-latest

%build
%meson
%meson_build

%install
%meson_install

%files
%defattr (-,root,root)
%{_prefix}/bin/coco-uploader
%{_prefix}/include/*
%{_prefix}/lib64/*

%changelog
* Sun Nov 06 2022 xuthus5 <xuthus5@gmail.com>
- first release
