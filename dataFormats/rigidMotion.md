# Rigid motion

## Overview

This data format represents a rigid motion.
It consists of a rotation and a translation.

The file contains a single JSON object, a map:

- **alpha** : rotation around x-axis in radians
- **beta** : rotation around y-axis in radians
- **gamma** : rotation around z-axis in radians
- **ox** : x-coordinate of center of rotation
- **oy** : y-coordinate of center of rotation
- **oz** : z-coordinate of center of rotation
- **tx** : translation in x-direction
- **ty** : translation in y-direction
- **tz** : translation in z-direction

## Example

````json
{
	"alpha" : -0.00053947427902487619,
	"beta" : 0.0018101838241351895,
	"gamma" : -0.0028546520358166973,
	"ox" : 142.96875,
	"oy" : 49.21875,
	"oz" : 62.5,
	"tx" : 0.20996151229174539,
	"ty" : -0.029118502792539375,
	"tz" : 0.077504038825920635
}
```

