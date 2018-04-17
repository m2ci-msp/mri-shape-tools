# landmark-tool

## Introduction

This tool provides a GUI for inspecting and annotating MRI scans.
Furthermore, it can be used to apply different filters to the scan, compute and show segmentations, and save visualizations of slices.
The tool currently inputs scan data in [JSON format][1].

## Installation

### Requirements

- [armadillo](http://arma.sourceforge.net)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [cmake](https://cmake.org)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
- [gtkmm-3.0](https://www.gtkmm.org)

### Installation Process

- configuration, adapt the installation path if needed
```sh
cmake cmake/CMakeLists.txt -DCMAKE_INSTALL_PREFIX=$HOME/usr
```
- installation
```sh
make install
```
## Usage

Launch the GUI by using the following command:

```sh
landmark-tool
```

After you opened a scan file, you can select between different view modes:

- **XY** : Sagittal view
- **YZ** : Coronal view
- **XZ** : Transverse view

You also can adjust the zoom factor of the visualization.
Additionally, opening a file activates other actions to be performed:

#### Placing landmarks

Place landmarks on the scan by left clicking on the desired location.
Newly added landmarks will appear in a list on the right.
Selecting them there will change the view to the slice the landmark is located on.
Double clicking on the list item will let you change the name of the landmark.

You can also move an existing landmark to a new location:
Highlight it by either using a right click near it or selecting it in the list.
Afterwards, press *F1* on the keyboard to activate the *move mode*.
Left clicking on a location will move the selected landmark there.
When you are done, press *F1* again to deactivate the *move mode*.

#### Saving and loading Landmarks

The placed landmarks can be saved to a JSON file.
Moreover, previously saved landmarks can also be loaded and added to the current scan.

#### Saving a slice visualization

The current slice visualization can be saved via **File->Save slice visualization** to a PNG file.

#### Applying filters to the scan

The menu item **View->Show filter window** gives you access to a window where you can apply filters to the scan by providing [image modification specifications][2] in a text input area.
Hitting **Apply** will apply the given specifications.
**Reset** will restore the original image.
**Save** can be used to save the specifications to a file.
**ATTENTION:** 
The **action** type **segment** with type **with landmarks** has a special behavior in this tool: 
Providing landmarks in the **options** field of this action is here **optional***.
By default, the landmarks placed in the GUI are be used.

#### Visualizing a segmentation

Use **File->Load segmentation** to open a previously computed segmentation of the scan.
A segmentation is in our case a scan of the same size as the original one that uses only two colors: 
One for the foreground (bright) and one for the background (dark).
The loaded segmentation is then overlaid with the original scan visualization where red represents the foreground and blue the background. 
You can adjust the transparency of the segmentation with **Mask Alpha**. 

#### Applying a segmentation to the scan

You can also compute a segmentation from within the tool:
**View->Show segmentation window** gives you access to a window with a similar functionality like the filter window.
This time, however, the provided modification specifications are used to compute a segmentation mask.

[1]: ../dataFormats/scan.md
[2]:../dataFormats/imageModifySpecifications.md
