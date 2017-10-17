
# Introduction

This document describes the JSON data format that is used to represent EMA data.

# Structure

The JSON file consists of a single map. In the following, we discuss the different components of the map that can be accessed by using the corresponding keys.

## *channels*

This entry of the map contains another map that describes the data associated to the individual coils of the EMA recordings.

### *position*

This part of an EMA coil consists of a list of positions. Each position describes the coordinate of the EMA coil at a specific time frame. The coordinate itself is stored in serialized manner. Given a time index `timeIndex` and the position array `position`, we can access the coordinate entries as follows:

```c++
double x = position[timeIndex * 3 + 0];
double y = position[timeIndex * 3 + 1];
double z = position[timeIndex * 3 + 2];
```

### *RMS*

This component of the EMA coil map contains a list of RMS values that correspond to the individual time frames.

## *timestamps*

This part of the main map contains the list of time stamps that belong to the time frames.

## *samplingFrequency*

This value describes the sampling frequency of the data in Hertz.

# Example

An example of such a JSON file is given by:

```json
{

    "channels" : {

        "TongueTip" : {

            "position" : [
                0, 1, 2,
                1, 1, 2,
                2, 2, 3
            ],

            "RMS" : [
                0.2,
                0.4,
                0.2
            ]

        },

        "TongueBack" : {

            "position" : [
                5, 5, 7,
                6, 9, 6,
                7, 8, 1
            ],

            "RMS" : [
                0.2,
                0.4,
                2
            ]

        }
    },
    "timestamps" : [
        0.005,
        0.010,
        0.015
    ],

    "samplingFrequency" : 200

}
```
