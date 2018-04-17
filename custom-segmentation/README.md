# custom-segmentation

## Introduction

This tool uses provided [image modification specifications][1] to segment a given scan into a tissue region (colored in white in the result) and background region (black).
Optionally, it can output a point cloud that consists of the surface points of the tissue region.
For these surface points, normals are computed that point to the outside of the tissue region.
The tool currently only inputs and outputs scan data in [JSON format][2].

**NOTE**: You can use the [landmark-tool][3] for testing out specifications.

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
image-modify --input string --output string --specifications string
```

- **input** : input scan.
- **output** : output mesh.
- **specifications** : file containing the image modifications to be applied.

[1]: ../dataFormats/imageModifySpecifications.md
[2]: ../dataFormats/scan.md
[3]: ../landmark-tool
