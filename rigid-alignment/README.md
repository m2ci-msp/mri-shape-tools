# rigid-alignment

## Introduction

This tool estimates the rigid motion between two meshes and uses the result to align both.
The tool currently supports meshes in OBJ and PLY format.

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
    ./rigid-alignment --source string --target string --output string [ --change string =  ]
                    [ --noTranslation ] [ --noRotation ] [ --noScaling ]
```

- **source** : source mesh
- **target** : target mesh
- **output** : result mesh
- **change** : applies the estimated motion to this mesh instead
- **noTranslation** : deactivates translation
- **noRotation** : deactivates rotation
- **noScaling** : deactivates scaling
