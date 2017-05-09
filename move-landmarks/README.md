# move-landmarks

## Introduction

Estimates the rigid motion between two meshes and applies this motion to provided landmarks.
Currently, the tool supports meshes in OBJ and PLY format.

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
./move-landmarks --source string --landmarks string --target string --output string
               [ --noTranslation ] [ --noRotation ] [ --noScaling ]
```

- **source** : source mesh
- **landmarks** : landmarks in [JSON format](1)
- **target** : target mesh
- **output** : transformed landmarks
- **noTranslation** : deactivate translation
- **noRotation** : deactivate rotation
- **noScaling** : deactivate scaling

[1]: ../dataFormats/landmarks.md
