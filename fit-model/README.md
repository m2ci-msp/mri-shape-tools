# fit-model

## Introduction

This tool fits a multilinear model to a target point cloud.

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
./fit-model --target string --model string --output string [ --landmarks string =  ]
          [ --landmarkTermWeight double = 1 ] [ --useLandmarksOnlyForInitialization ]
          [ --iter int = 10 ] [ --priorSize double = 1 ] [ --convergenceFactor int = 10000000 ]
          [ --projectedGradientTolerance double = 1e-05 ] [ --maxFunctionEvals int = 1000 ]
          [ --maxDistance double = 5 ] [ --maxAngle double = 60 ] [ --searchRadius double = 0 ]
          [ --fixedNeighbors ] [ --useNoProjection ]
```

- **target** : target point cloud. Supported formats: OBJ and PLY.
- **output** : fitted model mesh. Supported formats: OBJ and PLY.
- **model**  : model file in [YAML format][1].
- **landmarks** : landmarks in [JSON format][2].
- **landmarkTermWeight** : weight for the landmark energy.
- **useLandmarksOnlyForInitialization** : the landmark term is only used for initialization.
- **iter** : iterations to perform. In each iteration, the nearest neighbors are updated.
- **priorSize** : amount the model may move away from the mean in terms of standard deviation.
- **maxDistance** : maximally allowed distance for nearest neighbors to be considered.
- **maxAngle** : maximally allowed angle difference between nearest neighbor normal and current model mesh normal to be considered.
- **searchDistance** : search radius for adaptive nearest neighbor search. If this settings is used, nearest neighbors are preferred that lie below the current model mesh surface.
- **fixedNeighbors** : no nearest neighbor search is performed. If this setting is used, a one-to-one correspondence between model mesh and target vertices is assumed.
- **useNoProjection** : deactivate the projection of the model vertex onto the normal plane of the nearest neighbor and use nearest neighbor directly.

The remaining settings are specific to the [LBFGS solver of ITK][3].

[1]: ../dataFormats/model.md
[2]: ../dataFormats/landmarks.md
[3]: https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
