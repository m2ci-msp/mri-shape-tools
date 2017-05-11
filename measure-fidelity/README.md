# measure-fidelity

## Introduction

Measures how good a derived multilinear model can reproduce the orignal data.

## Installation

### Requirements

- [ann](https://www.cs.umd.edu/~mount/ANN)
- [armadillo](http://arma.sourceforge.net)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [itk](https://itk.org)
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
    ./measure-fidelity --model string --meshes string --output string [ --priorSize double = 1 ]
                     [ --convergenceFactor int = 10000000 ] [ --projectedGradientTolerance double = 1e-05 ]
                     [ --maxFunctionEvals int = 1000 ]
```

- **model**  : model file in [YAML format][1]
- **meshes** : sample database in [YAML format][2]
- **output** : mean and standard deviation of the error in JSON format
- **priorSize** : amount the model may move away from the mean in terms of standard deviation.

The remaining settings are specific to the [LBFGS solver of ITK][3].

[1]: ../dataFormats/model.md
[2]: ../dataFormats/sampleDatabase.md
[3]: https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
