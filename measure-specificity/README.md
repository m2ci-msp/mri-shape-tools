# measure-specificity

## Introduction

Measures how good randomly generated meshes from a multilinear model resemble the meshes in the used sample database.

## Installation

### Requirements

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
    ./measure-specificity --input string --output string --truncatedSpeaker int --truncatedPose int
                        [ --sampleAmount int = 1000 ] [ --subsets string list ]
                        [ --disableSpeaker ] [ --disablePhoneme ] [ --evaluateFixedPhoneme ]
```

- **input** : sample database in [YAML format][1]
- **output** : statistics of the measured errors in JSON format
- **truncatedSpeaker** : truncate speaker mode dimension to provided value
- **truncatedPose** : truncate phoneme mode dimension to provided value
- **sampleAmount** : amount of random samples to generate
- **subsets** : list of JSON files representing subsets of the generated mesh. Errors are then evaluated and output for each subset individually.
- **disableSpeaker** : disable the evaluation of the speaker specificity
- **disablePhoneme** : disable the evaluation of the phoneme specificity
- **evaluateFixedPhoneme** : evaluate speaker specificity while fixing the phoneme weights to the ones present in the sample database. Results are output for each phoneme individually. This option can also be combined with the __subsets__ flag.

## Subset JSON file format

Each file consists of one JSON object containing a name and a list of indices associated with this vertex subset.

### Example

```json
{
  "name": "tip",
  "indices": [
    2158,
    2237,
    2238,
    2239,
    2240,
    2243,
    2257,
    2316,
    2317,
    2318,
    2319,
    2320,
    2321,
    2335,
    2393,
    2394,
    2395,
    2396,
    2397,
    2398,
    2412,
    2459,
    2470,
    2471,
    2472,
    2473,
    2474,
    2475,
    2490,
    2535,
    2545,
    2546,
    2547,
    2548,
    2549,
    2550,
    2551,
    2566,
    2608,
    2618,
    2619,
    2620,
    2621,
    2622,
    2623,
    2624,
    2682,
    2692,
    2693,
    2694,
    2695,
    2696,
    2697,
    2698,
    2761,
    2762,
    2763,
    2764,
    2765,
    2827,
    2828,
    2829,
    2830
  ]
}
```
[1]: ../dataFormats/sampleDatabase.md
