# match-template

## Introduction

Performs template matching by applying the approach of [Wuhrer et al.][1].
By default, only the one-ring neighbors of a vertex are used for evaluating the smoothness energy.
Currently, meshes in OBJ and PLY format are supported.

## Installation

### Requirements

- [ann](https://www.cs.umd.edu/~mount/ANN)
- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
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
    ./match-template --target string --source string --output string [ --landmarks string =  ]
                   [ --meshNeighborhood string =  ] [ --smoothnessTermWeight double = 1 ]
                   [ --smoothnessTermWeightEnd double = 1 ] [ --postSmoothnessTermWeight double = 0 ]
                   [ --landmarkTermWeight double = 1 ] [ --landmarkTermWeightEnd double = 1 ]
                   [ --geodesicNeighbors double = 2 ] [ --sphericalNeighbors double = 2 ]
                   [ --iter int = 10 ] [ --convergenceFactor int = 10000000 ]
                   [ --projectedGradientTolerance double = 1e-05 ] [ --maxFunctionEvals int = 1000 ]
                   [ --maxDistance double = 5 ] [ --maxAngle double = 60 ] [ --searchRadius double = 0 ]
                   [ --fixedNeighbors ] [ --useNoProjection ]
```

- **target** : target point cloud
- **source**  : template mesh
- **output** : matched mesh
- **landmarks** : landmarks in [JSON format][2]
- **meshNeighborhood** : JSON file containing precomputed neighborhoods of the mesh vertices used in the smoothness term
- **smoothnessTermWeight** : weight for the smoothness energy
- **smoothnessTermWeightEnd** : weight for the smoothness energy in the last iteration, weight will linearly decrease.
- **postSmoothnessTermWeight** : weight for the smoothness energy in the postsmoothing operation that will be activated if this flag is provided
- **landmarkTermWeight** : weight for the landmark energy
- **landmarkTermWeightEnd** : weight for the landmark energy in the last iteration, weight will linearly decrease.
- **geodesicNeighbors** : add geodesic neighbors within the provided distance to the vertex neighbors that are used for evaluating the smoothness energy
- **sphericalNeighbors** : add neighbors within a sphere with the provided radius to the vertex neighbors that are used for evaluating the smoothness energy
- **iter** : iterations to perform. In each iteration, the nearest neighbors are updated.
- **maxDistance** : maximally allowed distance for nearest neighbors to be considered.
- **maxAngle** : maximally allowed angle difference between nearest neighbor normal and current model mesh normal to be considered.
- **searchDistance** : search radius for adaptive nearest neighbor search. If this settings is used, nearest neighbors are preferred that lie below the current model mesh surface.
- **fixedNeighbors** : no nearest neighbor search is performed. If this setting is used, a one-to-one correspondence between model mesh and target vertices is assumed.
- **useNoProjection** : deactivate the projection of the model vertex onto the normal plane of the nearest neighbor and use nearest neighbor directly.

The remaining settings are specific to the [LBFGS solver of ITK][3].

[1]: https://arxiv.org/abs/1306.4478
[2]: ../dataFormats/landmarks.md
[3]: https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
