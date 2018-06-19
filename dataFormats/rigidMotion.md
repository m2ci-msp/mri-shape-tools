# Rigid motion

## Overview

This data format represents a rigid motion.
It consists of a rotation and a translation.
Optionally, the center of rotation can be provided.
If it is omitted, the center of rotation is set to the center of the data to be rotated.

The file contains a single JSON object, a map:

- **alpha** : rotation around x-axis
- **beta** : rotation around y-axis
- **gamma** : rotation around z-axis
- **angleUnit** : unit of rotation angles, radians (default) or degrees
- **ox** : x-coordinate of center of rotation (optional)
- **oy** : y-coordinate of center of rotation (optional)
- **oz** : z-coordinate of center of rotation (optional)
- **tx** : translation in x-direction
- **ty** : translation in y-direction
- **tz** : translation in z-direction

## Example

```json
{
	"alpha" : -0.00053947427902487619,
	"beta" : 0.0018101838241351895,
	"gamma" : -0.0028546520358166973,
    "angleUnit" : "radians",
	"ox" : 142.96875,
	"oy" : 49.21875,
	"oz" : 62.5,
	"tx" : 0.20996151229174539,
	"ty" : -0.029118502792539375,
	"tz" : 0.077504038825920635
}
```
