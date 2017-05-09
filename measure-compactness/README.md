# measure-compactness

## Introduction

Measures the compactness of the provided mutlilinear model.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [cmake](https://cmake.org)

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
    ./measure-compactness --input string --output string
```

- **input**  : model file in [YAML format][1].
- **output** : measured compactness in JSON format.

[1]: ../dataFormats/model.md
