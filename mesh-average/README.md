# mesh-average

## Introduction

Averages a list of given meshes and outputs the result.
The meshes should have the same vertex and face sets.
Currently, the tool supports meshes in OBJ and PLY format.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
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
    ./mesh-average --input string list --output string
```

- **input** : list of meshes
- **output** : averaged mesh
