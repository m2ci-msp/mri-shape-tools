
# Converting tongue model weights to ema json

## Purpose

This tool converts a provided sequence of tongue model weights into EMA data.

## Installation

### Prerequisites
- jsoncpp
- armadillo
- yamlcpp

### Installation process

- configuring (**don't forget to adapt the path**)
```sh
cmake cmake/CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/work/tools/local
```
- installing
```sh
make install
```

## Using it
```sh
./cmake/weights-to-ema-json --input string --model string --output string --coilConfig fileName --unit string
```

- **input**: the weights json file
- **model**: the tongue model in the yaml format
- **output**: the produced ema json file
- **coilConfig**: file containing information about the coil configuration
- **unit**: the unit of the output coordinates. Supported units are *mm* (default), *cm*, *dm*, and *m*.

## File formats

### Input weights

The input file is a JSON file that contains a list of time frames.
Each time frames consists of the following information:

```json
{
    "speakerWeights" : [ ... ],
    "phonemeWeights" : [ ... ],
    "timeStamp" : 0
}
```

***speakerWeights*** and ***phonemeWeights*** refer to the tongue model weights.
The ***timeStamp*** provides an ID for the time frame.

### Coil configuration

The coil configuration is a JSON map:

```json
{
    "referenceName" : "ref",
    "coils" : [
	{ "coilName" : "T1", "vertexId" : 2697 },
	{ "coilName" : "T2", "vertexId" : 2059 },
	{ "coilName" : "T3", "vertexId" : 1249 },
    ]
}
```

The ***referenceName*** determines the name for the reference coil.
The tool generates a coil with this name and always uses the position (0, 0, 0) for it.

The ***coils*** entry contains a list of coil -> tongue mesh vertex correspondences:
The tool will use the vertex with the given ***vertexId*** of the generated tongue shape to produce the position of the coil ***coilName*** for the given time frame.

### Output EMA

EMA data is generated in the following format.
Basically, it is a map of maps such that

```groovy
json["channels"][LABEL]["position"]
```

represents a list of the serialized coordinates for the EMA coil with name LABEL. The list contains the x, y, and z coordinates for each time step in sequential order.

The list of time stamps of the recording can be obtained from

```groovy
json["timeStamps"]
```

Example file:

```json
{
  "channels" : {
    "LABEL" : {
      "position" : [
        x, y, z,
        x, y, z,
        .
        .
        .
      ],
    "LABEL2": {
      "position" : [ ... ]
    },
    .
    .
    .
  },
  "timeStamps" : [ 0, 1, 2, ... ]
}
```
