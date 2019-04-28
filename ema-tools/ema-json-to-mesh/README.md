# ema-json-to-mesh

## Introduction

This tool collects all positional data of given EMA channels and outputs mesh representations of the resulting point clouds.
The tool currently only supports EMA data in [JSON format][1]

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
ema-json-to-mesh --input string --channels string list --output string
```

- **input** : input EMA file.
- **channels** : id of the EMA channels that should be converted to point clouds.
- **output** : base name of the output files. Each channel is output separately.

[1]: ../../dataFormats/emaFormat.md
