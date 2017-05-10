# Landmarks JSON format

## Overview

The JSON file consists of a list of landmark objects.
Each landmark object contains the following values:

- **name** : The name of the landmark
- **sourceIndex** : vertex index of the source mesh. This index can be derived manually or by using the [landmark-indexer tool](../landmark-indexer).
- **sourceX, sourceY, sourceZ** : coordinates of the associated vertex on the source mesh.
- **targetX, targetY, targetZ** : target coordinates for the considered landmark

## Example

```json
[
    {
        "name": "Airway",
        "sourceIndex": 612,
        "sourceX": 28.021099090576172,
        "sourceY": 24.459999084472656,
        "sourceZ": -0.7873389720916748,
        "targetX": 168.03125,
        "targetY": 259.46875,
        "targetZ": 26.999998390674
    },
    {
        "name": "BackCenter",
        "sourceIndex": 816,
        "sourceX": 24.2460994720459,
        "sourceY": -4.89493989944458,
        "sourceZ": -0.002657880075275898,
        "targetX": 157.34375,
        "targetY": 234.53125,
        "targetZ": 26.999998390674
    },
    {
        "name": "FrontBaseCenter",
        "sourceIndex": 1935,
        "sourceX": -32.28990173339844,
        "sourceY": 2.9363300800323486,
        "sourceZ": 0.5320150256156921,
        "targetX": 107.46875,
        "targetY": 258.28125,
        "targetZ": 26.999998390674
    },
    {
        "name": "SurfaceCenter",
        "sourceIndex": 2069,
        "sourceX": -7.288000106811523,
        "sourceY": -20.43670082092285,
        "sourceZ": -0.2988469898700714,
        "targetX": 127.65625,
        "targetY": 229.78125,
        "targetZ": 26.999998390674
    },
    {
        "name": "Tip",
        "sourceIndex": 2548,
        "sourceX": -39.80419921875,
        "sourceY": -15.428400039672852,
        "sourceZ": -0.1735450029373169,
        "targetX": 100.34375,
        "targetY": 241.65625,
        "targetZ": 26.999998390674
    }
]
```
