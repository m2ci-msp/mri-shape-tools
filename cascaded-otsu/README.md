# cascaded-otsu

## Introduction

This tool uses [Otsu's method][1] to segment an MRI scan into a tissue region (colored white in the result) and a background region (black).
Furthermore, it may iterate this process to improve the result.
In each iteration, the colors in the tissue region are replaced by the average value in this region.

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
./cascaded-otsu --cascadeAmount int --source string --output string [ --cloud string =  ] [ --flip ]
```

- **cascadeAmount** : amount of iterations to perform
- **source** : input MRI scan
- **output** : segmented scan
- **cloud** : output name for point cloud. Supported output formats: OBJ and PLY.
- **flip** : flip the output normals in the point cloud.

[1]: https://en.wikipedia.org/wiki/Otsu%27s_method
[2]: ../dataFormats/scan.md
