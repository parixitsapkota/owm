# OWM
`OWM` is an extremely fast and small window manager for X11 based on dwm.

## Requirements
In order to build `owm` you need the Xlib header files.  
For `OWM` Build `voidlinux`
```bash
sudo xbps-install -S base-devel libX11-devel libXft-devel freetype-devel fontconfig-devel
```

## Installation
Edit Makefile to match your local setup.
```bash
sudo make install MODE=release
```

## Running owm
For `OWM` Runtime `voidlinux`
```bash
sudo xbps-install xorg-minimal xinit
```
Add the following line to your `.xinitrc` to start `OWM` using startx:
```bash
exec owm &
```
