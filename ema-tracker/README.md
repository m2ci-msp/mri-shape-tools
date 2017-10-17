#ema-tracker

## Introduction

This tool fits uses a [multilinear model][1] to track EMA data stored in [JSON format][2].

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
 ./ema-tracker --input string --model string --output string --channels string list
               --sourceIds int list [ --fixedSpeakerWeights string =  ] [ --reference string =  ]
             [ --scaleFactor double = 1 ] [ --shiftX double = 0 ] [ --shiftY double = 0 ]
             [ --shiftZ double = 0 ] [ --enforceMidsagittal ] [ --speakerWeight double = 1 ]
             [ --phonemeWeight double = 1 ] [ --meanBiasWeight double = 0 ]
             [ --priorSize double = 1 ] [ --convergenceFactor int = 10000000 ]
             [ --projectedGradientTolerance double = 1e-05 ] [ --maxFunctionEvals int = 1000 ]
```

- **input**    : EMA input file. See description below for expected format.
- **model**    : YAML file containing the multilinear model.
- **output**   : JSON output file.
- **sourceIds** : Vertex IDs of the model (in ascending order) corresponding to the provided channel IDs.
- **channels**  : Channel IDs in the EMA file.
- **reference** : ID of the reference coil.
- **fixedSpeakerWeights** : JSON file containing a list of weights that are used to derive a PCA model from the provided multilinear model.
- **scaleFactor** : Scaling factor to be applied to the EMA data.
- **priorSize** : Factor describing how far the model parameters may be away from the mean in terms of standard deviation
- **speakerWeight** : Weight for the speaker smoothness term
- **phonemeWeight** : Weight for the phoneme smoothness term

The remaining settings are specific to the [LBFGS solver of ITK][3].

# Output File Format

Per time frame, the following data is output as part of a list:

```json
{
	"energy" : 1,
	"phonemeVariations" : [ . . . ],
	"phonemeWeights" : [ . . . ],
	"speakerVariations" : [ . . . ],
	"speakerWeights" : [ . . . ],
	"target" :
	[
		{
			"x" : 0,
			"y" : 0,
			"z" : 0
		},
		{
			"x" : 1,
			"y" : 3,
			"z" : 2
		},
		{
			"x" : 1,
			"y" : 3,
			"z" : 2
		}
	],
	"timeStamp" : 0
}
```

- __energy__ : Energy after fitting of current time frame.
- __speakerVariations__ : Speaker variations, i.e., weights in terms of standard deviation.
- __phonemeVariations__ : Phoneme variations, i.e., weights in terms of standard deviation.
- __speakerWeights__ : Speaker weights.
- __phonemeWeights__ : Phoneme weights.
- __target__ : Positional data of the chosen channels.
- __timeStamp__ : Time stamp of the processed frame.

[1]: ../dataFormats/model.md
[2]: ../dataFormats/emaFormat.md
[3]: https://public.kitware.com/vxl/doc/release/core/vnl/html/classvnl__lbfgsb.html
