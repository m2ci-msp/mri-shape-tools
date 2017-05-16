# measure-generalization

## Introduction

Measures how good a derived multilinear model generalizes to unknown data by excluding samples from a provided data base.

## Installation

### Requirements

- [ann](https://www.cs.umd.edu/~mount/ANN)
- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [itk](https://itk.org)
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
   ./measure-generalization --input string --output string [ --priorSize double = 1 ]
                             [ --convergenceFactor int = 10000000 ] [ --projectedGradientTolerance double = 1e-05 ]
                             [ --maxFunctionEvals int = 1000 ] [ --restrictedSpeaker int = 0 ]
                             [ --restrictedPhoneme int = 0 ] [ --minComponents int = 1 ]
```

- **input** : sample database in [YAML format][1]
- **output** : statistics of the measured errors in JSON format
- **priorSize** : amount the model may move away from the mean in terms of standard deviation.
- **restrictedSpeaker** : truncate speaker mode dimension to provided value during the evaluation of the phoneme generalization
- **restrictedPhoneme** : truncate phoneme mode dimension to provided value during the evaluation of the speaker generalization
- **minComponents** : the smallest dimension of a mode to consider during the evaluation

The remaining settings are specific to the [LBFGS solver of ITK][2].

[1]: ../dataFormats/sampleDatabase.md
[2]: https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
