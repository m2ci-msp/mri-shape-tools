# Changelog

## version 1.1

- major internal rewrite to unify scan and image data structures
- several fixes to deal with compiler warnings
- added tools: [lucas-kanade][1], [apply-rigid-motion][2], [scan-crop-gui][3], [custom-segmentation][4], [image-modify][5], [threshold][6]
- renamed tool: adaptive-threshold -> otsu-threshold
- removed measure-fidelity tool
- removed scan-alignment tool, superseded by lucas-kanade tool 
- added more image filters, see [image modification specifications][7] for an overview
- added filter and segmentation functionality to [landmark-tool][8]
- GUI overhaul for landmark-tool
- added '--scale_to_8bit' flag to otsu-threshold
- added data formats: [image-landmarks][9], [rigid body motion][10], [image modification specifications][7]
- data format update: scan origin is now stored in form of scan coordinates instead of voxel coordinates

## version 1.0

initial release

[1]:./lucas-kanade
[2]:./apply-rigid-motion
[3]:./scan-crop-gui
[4]:./custom-segmentation
[5]:./image-modify
[6]:./threshold
[7]:./dataFormats/imageModificationSpecifications.md
[8]:./landmark-tool
[9]:./dataFormats/imageLandmarks.md
[10]:./dataFormats/rigidMotion.md

