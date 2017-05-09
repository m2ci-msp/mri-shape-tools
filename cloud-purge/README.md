# cloud-purge

## Introduction

This tool provides several options to remove points from a given point cloud.
## Installation

### Requirements

- [ann](https://www.cs.umd.edu/~mount/ANN)
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
   ./cloud-purge --source string --output string [ --landmarks string =  ] [--boundaryMesh string =  ]
               [ --keepPointsBelow ] [ --maxDistance double = 1 ] [ --bounceSides ]
               [ --sideDistance double = 3 ]
```

- **source** : input point cloud. Supported formats: OBJ and PLY.
- **output** : modified point cloud.
- **landmarks** : landmarks in [JSON format][2]. Every point below the line connecting __FrontBaseCenter__  and __Airway__ is removed.
- **boundaryMesh** : boundary mesh. Every point above the normal plane of its nearest neighbor in the boundary mesh is removed.
- **keepPointsBelow** : used in combination with __boundaryMesh__. Keeps points below the boundary mesh, even if they are above some normal plane.
- **maxDistance** : used in combination with __boundaryMesh__. Keeps points above the normal plane whose distance to the plane is below the given value.
- **bounceSides** : used in combination with __boundaryMesh__. Removes points that are left and right of the bounding box of the boundary mesh.
- **sideDistance** : used in combination with __boundaryMesh__ and __bounceSides__. Points within the given distance from the boundary box are kept.

[1]: https://en.wikipedia.org/wiki/Otsu%27s_method
[2]: ../dataFormats/landmarks.md
