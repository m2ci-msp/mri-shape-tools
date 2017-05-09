# extract-slice

## Introduction

This tool extracts 2D image planes from a given 3D MRI scan in PNG format.
The tool currently only inputs and outputs scan data in [JSON format][1].

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
- [cairomm-1.0](https://www.cairographics.org/cairomm)

### Installation Process

- configuration, adapt the installation path if needed
```sh
cmake cmake/CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/usr/bin
```
- installation
```sh
make install
```
## Usage

```sh
./extract-slice --input string --output string [ --xy int list ] [ --xz int list ]
              [ --yz int list ] [ --discard ]
```

- **input** : input MRI scan
- **output** : base name for output
- **xy** : list of xy slices to extract
- **xz** : list of xz slices to extract
- **yz** : list of yz slices to extract
- **discard** : discard 0.25 percent of the brightest and darkest voxels. This helps to improve the contrast of the result.

[1]: ../dataFormats/scan.md
