Name:           coco-uploader
Version:        0.0.1
Release:        1%{?dist}
Summary:        simple upload plugin.

License:        MIT
URL:            https://github.com/xuthus5/coco
Source0:        https://github.com/xuthus5/coco.git

BuildRequires:  meson
BuildRequires:  gtk4-devel
BuildRequires:  curl-devel
Requires:       

%description


%prep
%autosetup
rm -rf build

%build
meson setup build
meson compile -C build


%install
DESTDIR=%{buildroot} ninja-build -v install


%files
%license add-license-file-here
%doc add-docs-here



%changelog
* Sun Nov 06 2022 xuthus5 <xuthus5@gmail.com>
- 
