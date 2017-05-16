# procrustes-alignment

## Introduction

Applies [procrustes alignment][1] to a collection of meshes to remove rigid motions among them.
The tool currently supports meshes in OBJ and PLY format.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
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
./procrustes-alignment --input string list --output string list --iter int [ --noTranslation ]
                     [ --noRotation ] [ --noScaling ] [ --originIndex int = 0 ]
                     [ --mapping string = XYZ ] [ --xFactor double = 1 ] [ --yFactor double = 1 ]
                     [ --zFactor double = 1 ]
```

- **input** : input list of meshes
- **output** : output list of meshes
- **iter** : iterations to perform
- **noTranslation** : do not remove translations
- **noRotation** : do not remove rotations
- **noScaling** : do not remove scalings
- **originIndex** : after alignment, translate each mesh such that the vertex with the given index is at the origin
- **mapping** : specification how the axes should be interpreted, *e.g.*, **ZXY** means that the first axis corresponds to the *z-axis*, the second one to the *x-axis*, and the third one to the *y-axis*. The meshes are then changed such that they fulfill the specification **XYZ**.
- **xFactor**, **yFactor**, and **zFactor** : factors the original axes should be multiplied with.

[1]: https://en.wikipedia.org/wiki/Procrustes_analysis
