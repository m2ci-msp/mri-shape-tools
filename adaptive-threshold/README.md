# adaptive-threshold

## Introduction

This tool uses [Otsu's method][1] to segment an MRI scan into a tissue region (colored white in the result) and a background region (black).
Optionally, it can output a point cloud that consists of the surface points of the tissue region.
The tool currently only inputs and outputs scan data in [JSON format][2].

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
./adaptive-threshold --source string --output string [ --cloud string =  ] [ --flip ]
```

- **source** : input MRI scan
- **output** : segmented scan
- **cloud** : output name for point cloud. Supported output formats: OBj and PLY.
- **flip** : flip the output normals in the point cloud.

[1]: https://en.wikipedia.org/wiki/Otsu%27s_method
[2]: ../dataFormats/scan.md