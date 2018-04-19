# msp-tongue-shape-project

## Introduction

This repository contains the source code of the tools needed for our gradle framework that aims at deriving a multilinear tongue model from a given MRI data set.
Have a look at [our paper][1] to learn more about the background.

## Installation

You can install each tool individually by using the cmake files in the respective subdirectory.
However, you are also able to install them all at once from the root directory.

### Requirements

Please make sure that the following libraries and tools are available:

- A C++ compiler that can handle C++11 code, *e.g.*, GCC (since version 4.8.1) or Clang (since version 3.3).
- [ann](https://www.cs.umd.edu/~mount/ANN)
- [armadillo](http://arma.sourceforge.net)
- [cmake](https://cmake.org)
- [gtkmm-3.0](https://www.gtkmm.org)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
- [cairomm-1.0](https://www.cairographics.org/cairomm)
- [itk](https://itk.org)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

On Ubuntu, for example, you can install all dependencies by executing the following command:

```sh
apt-get install \
    build-essential \
    libann-dev \
    libarmadillo-dev \
    cmake \
    libjsoncpp-dev \
    pkg-config \
    libgtkmm-3.0-dev \
    libcairomm-1.0-dev \
    libinsighttoolkit4-dev \
    libyaml-cpp-dev
```

On OSX with [Homebrew](http://brew.sh), install the dependencies with

```sh
brew install \
    armadillo \
    jsoncpp \
    ann \
    cairomm \
    insighttoolkit \
    vtk \
    gtkmm3 \
    yaml-cpp
```

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

## Documentation

Documentation about the individual tools is available in their respective folders.
The [dataFormats](./dataFormats) folder contains information about the various data formats used by our tools.

## License

This work is licensed under the [MIT license](./LICENSE.md).

If you are using our tools, please cite, for the time being, the following paper:

```bibtex
@article{Hewer2018CSL,
  author = {Hewer, Alexander and Wuhrer, Stefanie and Steiner, Ingmar and Richmond, Korin},
  doi = {10.1016/j.csl.2018.02.001},
  eprint = {1612.05005},
  eprinttype = {arxiv},
  journal = {Computer Speech \& Language},
  month = sep,
  pages = {68-92},
  title = {A Multilinear Tongue Model Derived from Speech Related {MRI} Data of the Human Vocal Tract},
  url = {https://arxiv.org/abs/1612.05005},
  volume = {51},
  year = {2018}
}
```

[1]: http://arxiv.org/abs/1612.05005
