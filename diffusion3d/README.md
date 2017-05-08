# diffusion3d

## Introduction

This tool uses a 3D variant of the [edge-enhancing diffusion filter][1] to enhance a 3D MRI scan.
It is capable of removing noise and repairing missing surface information in the scan.
The tool currently only inputs and outputs scan data in [JSON format][2].

## Installation

### Requirements

- [armadillo][http://arma.sourceforge.net]
- [jsoncpp][https://github.com/open-source-parsers/jsoncpp]
- [cmake][https://cmake.org]

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
./diffusion3D --input string --output string [ --sigma double = 1 ] [ --rho double = 1 ] [ --lambda double = 1 ] [ --iter int = 1 ]
```

- **source** : input MRI scan
- **output** : scan after smoothing
- **iter** : number of iterations 
- **sigma** : standard deviation of the Gaussian for smoothing the scan before computing the derivatives 
- **rho** : standard deviation of the Gaussian for combining surface information in a neighborhood
- **lambda** : parameter that influences how high contrast differences have to be in order to be considered as surface boundaries

[1]: http://www.mia.uni-saarland.de/weickert/book.html
[2]: ../dataFormats/scan.md