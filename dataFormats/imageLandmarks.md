# Image landmarks JSON format

## Overview

The JSON file consists of a list of image landmark objects.
Each landmark object contains the following values:

- **name** : The name of the landmark
- **x, y, z** : voxel coordinates. These refer to the actual voxel indexes and do not take the grid spacings of the scan into account.

## Example

```json
[
	{
		"name" : "BackCenter",
		"x" : 84.5,
		"y" : 75.5,
		"z" : 41.5
	},
	{
		"name" : "SurfaceCenter",
		"x" : 63.5,
		"y" : 76.5,
		"z" : 41.5
	},
	{
		"name" : "Airway",
		"x" : 90.5,
		"y" : 89.5,
		"z" : 41.5
	},
	{
		"name" : "Tip",
		"x" : 49.5,
		"y" : 85.5,
		"z" : 41.5
	},
	{
		"name" : "FrontBaseCenter",
		"x" : 55.5,
		"y" : 93.5,
		"z" : 41.5
	}
]
```
