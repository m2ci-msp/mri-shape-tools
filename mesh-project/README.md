# mesh-project

## Introduction

Projects a mesh onto an 3D MRI scan by drawing samples on the edges and coloring the scan voxel white that contains the sample.
The tool inputs and outputs scan data in [JSON format][1].
Currently, meshes in OBJ and PLY format are supported.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
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
    ./mesh-project --mesh string --scan string --output string
```

- **mesh** : mesh to be projected
- **scan** : MRI scan
- **output** : output scan

[1]: ../dataFormats/scan.md
