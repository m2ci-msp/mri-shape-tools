# Specifications for image manipulation

## Introduction

This document describes the format of the image manipulation specifications that are used by the [image-modify][1], [custom-segmentation][2], and [landmark-tool][3] tools.

## Format

The specifications are provided as a list of JSON objects.

### Example

The following specifications would 

1. apply a dilation with radius 1 to the image data
2. equalize the histogram
3. apply a median filter with radius 1

```json
[
    {
        "action" : "morphology",
        "options" : {
            "type" : "dilation",
            "radius" : 1
        }
    },
    {
        "action" : "histogram",
        "options" : {
            "type" : "equalize"
        }
    },
    {
        "action" : "filter",
        "options" : {
            "type" : "median",
            "radius" : 1
        }
    }
]
```

We see that the specification objects consist of an **action** and an **options** field.
One action might offer different types of operations:
The wanted type is set in the **type** field of the **options** data.

## Actions

In this section, we present the supported actions and also discuss their options.
We use the name of the action as the heading of the associated subsection.

### crop

The action crops the image to a specified region of interest.
It offers currently only one operation.

Supported **options**:

- minX
- minY
- minZ
- maxX
- maxY
- maxZ

The indices (minX, minY, minZ) and (maxX, maxY, maxZ) determine the corner points of the box that is used as region of interest.

### edge

Action for visualizing edges.
In the following, we discuss the supported types of operation.

#### laplacian zero crossings

This operation visualizes detected zero crossings of the laplacian as white voxels in the image.
Everything else is shown in black.

#### gradient magnitude

Here, we visualize the magnitude of the image gradient in each voxel.

#### largest eigenvalue structure tensor

The last operation type shows the magnitude of the largest eigenvalue of the structure tensor in each voxel.
The radius of the box that is used for computing the structure tensor has to be set in the **radius** field of the **options** data.

### morphology

This action provides the basic operations of [mathematical morphology][4], the [top-hat transforms][5], and the [morphological gradient][6] that can be applied to the image.
All operations use a cube as structuring element.

Shared **options** for all operations of this action:

- **radius**: radius of the underlying structuring element

Supported **types** of operations:

- dilation
- erosion
- opening
- closing
- white top hat
- black top hat
- gradient

Please have a look at the above links for background information on these operations.

### filter

Action for applying filters to the image.
Supported **types**:

#### median

This operation applies a [median filter][7] to the image by using a box as the window for computing the median at each voxel.
The radius of this box has to be set in the **radius** field of the **options** data.

#### gaussian

This filter type applies a [Gaussian smoothing][8] to the image.
The standard deviation of the Gaussian kernel has to be set in the **sigma** field of the **options** data.

#### diffusion

Applies an [anisotropic diffusion filter][9] to the image.
The underyling implementation uses an explicit scheme.
Needed **options**:

- **timeSteps**: time steps to take for the evolution
- **stepSize**: time step size that affects the stability of the scheme ( use, for example, 0.124 )
- **contrastLambda**: parameter that determines which edges are preserved
- **integrationRho**: standard deviation of Gaussian kernel used for computing the structure tensor
- **presmoothSigma**: standard deviation of Gaussian kernel used for presmoothing the image

### interpolateSlice

Interpolates a slice at a given position and adds it to the image.
The slice is inserted between the slices that participated in the interpolation process.
The position is set in the **position** of the **options** data.

Supported types:

- xy
- xz
- yz

### histogram

Offers methods for modifying the histogram of the image.
Currently only one operation is supported:

#### equalize

This method [equalizes the histogram][10].

### segment

This action consists of several thresholding approaches that segment an image into a foreground region ( shown in white ) and a background region ( shown in black ).
Supported types:

#### threshold

Basic thresholding that uses the value in the field **threshold** of the **options** data to segment the image:
Everything below the threshold is classified as background, the remaining voxels as foreground.

#### otsu

This operation uses [Otsu's method][11] to segment the image.

#### iterated otsu

Applies Otsu's method multiple times.
After each iteration, the voxel colors in the foreground region are replaced with the mean in that area.
The number of iterations is set with **iterations** in the **options**.

#### with landmarks

Chooses the smallest threshold such that all provided landmarks are inside the foreground region.
Landmarks are provided as a list of image coordinates in the **landmarks** field of **options**:

```json
[
    {
        "action" : "segment",
        "options" : {
            "type" : "with landmarks",
            "landmarks" : [
              { "x" : 0, "y" : 2, "z" : 3 },
              { "x" : 2, "y" : 20, "z" : 10 },
              { "x" : 77, "y" : 81, "z" : 40 }
            ]
        }
    }
]
```

For convenience, you can use the [landmark-tool][3] to distribute the landmarks and export the corresponding specifications to a JSON file.

### values

Several operations that modify the voxel colors.

#### scale

Lineraly scales the values to have the provided minimum and maximum.
The new minimum is set in **minNew** in the **options**, the new maximum in **maxNew**.

#### normalize

Linearly scales the values to the interval [0, 1]

#### discard

This operation corresponds to the [contrast-stretch][12] command of image magick.
The percentage of brightest pixels that are set to white is provided in **upper** of the **options**.
The percentage of darkest pixels that are set to black is provided in **lower** of the **options**.

#### add

Adds a value to all voxel colors.
Optionally, corner points of a bounding box can be provided.
In this case, only voxels inside that box are modified.
The value to be added is given in **value** of the **options**.
The corner points are provided by **minX**, **minY**, **minZ**, **maxX**, **maxY**, and **maxZ**.

[1]:../image-modify
[2]:../custom-segmentation
[3]:../landmark-tool
[4]:https://en.wikipedia.org/wiki/Mathematical_morphology
[5]:https://en.wikipedia.org/wiki/Top-hat_transform
[6]:https://en.wikipedia.org/wiki/Morphological_gradient
[7]:https://en.wikipedia.org/wiki/Median_filter
[8]:https://en.wikipedia.org/wiki/Gaussian_blur
[9]:https://en.wikipedia.org/wiki/Anisotropic_diffusion
[10]:https://en.wikipedia.org/wiki/Histogram_equalization
[11]:https://en.wikipedia.org/wiki/Otsu%27s_method
[12]:http://www.imagemagick.org/Usage/color_mods/#contrast-stretch
