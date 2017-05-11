# Scan JSON format

## Overview

This data format is intended to represent a 3D MRI scan f.
Here, *f(x, y, z)* represents the measured value at [coronal slice][1] *x*, [transverse slice][2] *y*, and [sagittal slice][3] *z*.

The file contains a single JSON object, a map:

- **origin** : coordinate in the original scan the coordinate (0, 0, 0) corresponds to. This entry is modified by [scan-crop](../scan-crop).

- **dimensions** : the dimensions of the scan

- **spacings** : the grid spacings along the three axes

- **minimum** and **maximum** : the two exteme values in the data

- **data** : a list containing the measured values. *f(x, y, z)* is found at index *z ny nx + x ny + y* where *nx* and *ny* represent the dimensions along the *x*- and *y*-axis respectively.

## Example

```json
{
    "origin": {
        "x": 0,
        "y": 0,
        "z": 0
    },
    "dimensions": {
        "x": 2,
        "y": 2,
        "z": 2
    },
    "spacings": {
        "x": 1.2,
        "y": 1.2,
        "z": 1.1
    },
    "minimum": 0,
    "maximum": 255,
    "data": [
        0,
        0,
        255,
        0,
        255,
        0,
        0,
        0
    ]
}

```

[1]: https://en.wikipedia.org/wiki/Coronal_plane
[2]: https://en.wikipedia.org/wiki/Transverse_plane
[3]: https://en.wikipedia.org/wiki/Sagittal_plane
