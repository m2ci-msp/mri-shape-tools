# scan-alignment

## Introduction

This tool rigidly aligns a mesh obtained from a source scan to a target scan.
This is done by constructing color profiles along the normals at the mesh vertices and then finding a transformation such that the color profiles in the source scan and the ones in the target scan are similar.
Currently, meshes in PLY and OBJ format are supported.
The MRI scan has to be in [JSON format][1].

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
cmake cmake/CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/usr/bin
```
- installation
```sh
make install
```
## Usage

```sh
    ./scan-alignment --sourceMesh string --sourceScan string --targetScan string --output string
                     [ --minimizerOutput string =  ] [ --iter int = 10 ] [ --translationLimit double = 20 ]
                     [ --rotationLimit double = 20 ] [ --convergenceFactor int = 10000000 ]
                     [ --projectedGradientTolerance double = 1e-05 ] [ --maxFunctionEvals int = 1000 ]
                     [ --profileLength double = 5 ] [ --profileOffset double = 1 ]
                     [ --translationStep double = 1 ] [ --rotationStep double = 1 ]
```

- **sourceMesh** : mesh to be aligned
- **sourceScan** : scan the source mesh was obtained from
- **targetScan**  : scan the mesh should be aligned to
- **output** : aligned mesh
- **minimizerOutput** : outputs information about the minimizer in JSON format
- **iter** : iterations to perform
- **translationLimit** : limit the absolute value of the translation entries to the provided value
- **rotationLimit** : limit the absolute value of the rotation (in degrees) to the provided value
- **profileLength** : length of the color profile
- **profileOffset** : offset of the color profile from the surface of the mesh
- **translationStep** : step size for discretizing the translational derivative of the energy
- **rotationStep** : step size for discretizing the rotational derivative of the energy

The remaining settings are specific to the LBFGS solver of ITK.

[1]: ../dataFormats/scan.md
