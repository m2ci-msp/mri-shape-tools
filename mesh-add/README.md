# mesh-add

## Introduction

Adds two meshes together and outputs the result.
Currently, only meshes in OBJ and PLY format are supported.

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
./mesh-add --first string --second string --output string [ --onlyPoints ] [ --computeNormals ]
```

- **first** : first mesh
- **second** : second mesh
- **output** : resulting mesh
- **onlyPoints** : only uses vertex data
- **computeNormals** : computes the normals before saving the result
