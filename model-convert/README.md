# model-convert

## Introduction

Converts a multilinear model to a format usable by an HTML framework.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
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
    ./model-convert --model string --outputBaseName string
```

- **model** : model in [YAML format][1]
- **outputBaseName** : base name of output files

[1]: ../dataFormats/model.md
