# landmark-indexer

## Introduction

This tool assigns the source vertex indices to landmarks given an input mesh and the location of the source vertices.

## Installation

### Requirements

- [ann](https://www.cs.umd.edu/~mount/ANN)
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
 ./landmark-indexer --sourceMesh string --landmarks string --output string
```

- **sourceMesh** : input mesh the landmarks should be associated to.
- **landmarks** : landmarks in [JSON format][1].
- **output** : landmarks with vertex indices.

[1]: ../dataFormats/landmarks.md
