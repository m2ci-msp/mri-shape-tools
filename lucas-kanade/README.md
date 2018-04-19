# lucas-kanade

## Introduction

This tool uses a variant of the [approach by Lucas and Kanade][1] to estimate the rigid motion between two given scans.

The tool uses a box as region of interest that is being tracked.
This region is provided as the bounding box of some given landmarks.
We use the [Zero normalized cross-correlation][2] as penalizer.
This penalizer makes the approach robust against noise and brightness differences between the two scans.

The tool currently only inputs scan data in [JSON format][3].
The motion description is output in [rigid motion format][4].
During output, we add an additional field to it that shows the achieved correlation between the two scans during optimization.
As input format for the landmarks, we use the [image landmarks format][5].

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)
- [itk](https://itk.org)

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
lucas-kanade --source string --target string --landmarkFile string --output string
           [ --iterationAmount int = 10 ] [ --convergenceFactor int = 10000000 ]
           [ --projectedGradientTolerance double = 1e-05 ] [ --maxFunctionEvals int = 1000 ]
```

- **source** : source scan.
- **target** : target scan.
- **landmarkFile** : landmarks describing the region of interest.
- **output** : file for saving estimated rigid motion.
- **iterationAmount** : iterations to be used. In each iteration, the approach tries to find a motion increment that is added to the already found one.

The remaining settings are specific to the [LBFGS solver of ITK][6].

[1]:https://en.wikipedia.org/wiki/Lucas%E2%80%93Kanade_method
[2]:https://en.wikipedia.org/wiki/Cross-correlation#Zero-normalized_cross-correlation_(ZNCC)
[3]:../dataFormats/scan.md
[4]:../dataFormats/rigidMotion.md
[5]:../dataFormats/imageLandmarks.md
[6]:https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
