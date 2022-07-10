# Coco

A Developer Tool Building by GTK4(meson+gtk4+Adwaita).

## License

Coco is licensed under the MIT.

## Functions

- [x] base64 encode and decode
- [x] time convert
- [x] clipboard
- [x] text hash and file hash
- [x] password generate
- [x] file share and storage
- [ ] code pastebin

## Usage

install flatpak gnome platform environment or gnome-nightly flatpakrepo.

```sh
# need master or gnome platform 42 environment.
flatpak install org.gnome.Platform/x86_64/42
flatpak install org.gnome.Sdk/x86_64/42
# gnome-nightly include org.gnome.Platform/x86_64/master
# flatpak remote-add --if-not-exists gnome-nightly https://nightly.gnome.org/gnome-nightly.flatpakrepo
# flatpak install org.gnome.Platform/x86_64/master
# flatpak install org.gnome.Sdk/x86_64/master
```

### build

install flatpak-builder

```sh
sudo dnf install flatpak-builder -y
```

install cc.xuthus.coco

```sh
git clone https://github.com/xuthus5/coco.git
cd coco
flatpak-builder --user --install --force-clean build-flatpak/ cc.xuthus.coco.json
```

if install failed, please create local repo and reinstall

```sh
flatpak-builder --repo=build-repo --force-clean build-flatpak/ cc.xuthus.coco.json
flatpak --user remote-add --no-gpg-verify coco-repo build-repo
flatpak --user install coco-repo cc.xuthus.coco
```

run cc.xuthus.coco

```sh
flatpak run cc.xuthus.coco
```

## screenshot

![base64](screenshot/base64.png)

![time-convert](screenshot/time-convert.png)

![password-generate](screenshot/password-generate.png)

