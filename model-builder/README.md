# model-builder

## Introduction

Derives a multilinear model from a provided sample database.

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
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
    ./model-builder --samples string --output string [ --outputMesh string =  ] [ --outputPhonemeWeights string =  ]
                  [ --truncatedSpeaker int = 0 ] [ --truncatedPhoneme int = 0 ]
```

- **samples** : sample database in [YAML format][1]
- **output**  : model file in [YAML format][2]
- **outputMesh** : outputs the mean mesh of the model. Supported formats: OBJ and PLY.
- **outputPhonemeWeights** : outputs the original weights of the considered phonemes in YAML format
- **truncatedSpeaker** : truncates the speaker mode to the provided amount of entries
- **truncatedPhoneme** : truncates the phoneme mode to the provided amount of entries

[1]: ../dataFormats/sampleDatabase.md
[2]: ../dataFormats/model.md
