# msp-tongue-shape-project

## Introduction

This repository contains the source code of the tools needed for our gradle framework that aims at deriving a multilinear tongue model from a given MRI data set.


## Installation

You can install each tool individually by using the cmake files in the respective subdirectory.
However, you are also able to install them all at once from the root directory.

### Requirements

Please make sure that the following libraries and tools are available:

- [ann](https://www.cs.umd.edu/~mount/ANN)
- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
- [cairomm-1.0](https://www.cairographics.org/cairomm)
- [itk](https://itk.org)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

### Installation Process

In order to install all tools, perform the following steps:

- configuration, adapt the installation path if needed
```sh
cmake CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/usr
```
- installation
```sh
make install
```
