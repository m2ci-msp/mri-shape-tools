# landmark-alignment

## Introduction

This tool uses landmarks to rigidly transform a given mesh.
The tool currently supports meshes in OBJ and PLY format.

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
./landmark-alignment --source string --landmarks string --output string [ --noScaling ]
                   [ --noRotation ] [ --noTranslation ]
```

- **source** : input mesh
- **landmarks** : landmarks in [JSON format][1]
- **output** : transformed mesh
- **noScaling** : deactivates scaling
- **noRotation** : deactivates rotation
- **noTranslation** : deactivates translation

[1]: ../dataFormats/landmarks.md
