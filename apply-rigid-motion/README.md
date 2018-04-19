# apply-rigid-motion

## Introduction

This tool applies a provided rigid motion to a given mesh and outputs the result.
It inputs a motion description in [JSON format][1].

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
apply-rigid-motion --input string --motion string --output string
```

- **input** : input mesh. Supported formats: OBJ and PLY.
- **motion** : file containing the motion to be applied.
- **output** : output mesh. Supported formats: OBJ and PLY.

[1]: ../dataFormats/rigidMotion.md
