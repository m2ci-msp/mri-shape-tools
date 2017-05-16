# scan-crop

## Introduction

Crops an MRI scan to the provided bounding box.
The tool currently inputs and outputs scan data in [JSON format][1].

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)

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

```sh
    ./scan-crop --input string --output string --min int int int --max int int int
```

- **source** : input MRI scan
- **output** : cropped scan
- **min** : smallest coordinate of the bounding box
- **max** : largest coordinate of the bounding box

[1]: ../dataFormats/scan.md
