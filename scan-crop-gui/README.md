# scan-crop-gui

## Introduction

This tool provides a GUI for inspecting and saving cropping settings for scans.
The tool currently inputs scan data in [JSON format][1].
It uses an output format that is suitable as configuration for the [gradle framework][2]

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
- [gtkmm-3.0](https://www.gtkmm.org)

### Installation Process

- configuration, adapt the installation path if needed
```sh
cmake cmake/CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/usr
```
- installation
```sh
make install
```
## Usage

Launch the GUI by using the following command:

```sh
scan-crop-gui
```

[1]: ../dataFormats/scan.md
[2]:https://github.com/m2ci-msp/mri-shape-framework
