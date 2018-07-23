# Changelog

## version 1.2

- simplified computation of angular derivative in [match-template][13] tool
- added tool for performing energy-based rigid alignment: [energy-rigid-alignment][14]
- added feature to [match-template][13]: tool can now optionally apply a rigid alignment
- fixed bugs in stencil computation for diffusion filtering

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
- added JSON output functionality to [model-builder][11]
- added [JSON model dataformat description][12] 
- fix for [landmark-tool][8]: landmarks are no longer placed between two voxels

## version 1.0

initial release

[1]:./lucas-kanade
[2]:./apply-rigid-motion
[3]:./scan-crop-gui
[4]:./custom-segmentation
[5]:./image-modify
[6]:./threshold
[7]:./dataFormats/imageModifySpecifications.md
[8]:./landmark-tool
[9]:./dataFormats/imageLandmarks.md
[10]:./dataFormats/rigidMotion.md
[11]:./model-builder
[12]:./dataFormats/modelJson.md
[13]:./match-template
[14]:./energy-rigid-alignment
