# Coco

A Developer Tool Building by Adwaita.

## License

Coco is licensed under the MIT.

## Functions

- [x] base64 encode and decode
- [x] time convert
- [x] clipboard
- [ ] hash
- [ ] password generate

## Usage

step one: install gnome-nightly flatpakrepo.

```sh
# gnome-nightly include org.gnome.Platform/x86_64/master
flatpak remote-add --if-not-exists gnome-nightly https://nightly.gnome.org/gnome-nightly.flatpakrepo
# need master or gnome platform 42 environment.
flatpak install org.gnome.Platform/x86_64/master
```

step two: install coco

```sh
flatpak install --user cc.xuthus.cc.flatpak
```

