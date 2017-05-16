# landmark-tool

## Introduction

This tool provides a GUI for inspecting and annotating MRI scans.
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
./landmark-tool
```

After you opened a scan file, you can select between different view modes:

- **XY** : Sagittal view
- **YZ** : Coronal view
- **XZ** : Transverse view

#### Placing the Landmarks

Place landmarks on the scan by left clicking on the desired location.
Newly added landmarks will appear in a list on the right.
Selecting them there will change the view to the slice the landmark is located on.
Double clicking on the list item will let you change the name of the landmark.

You can also move an existing landmark to a new location:
Highlight it by either using a right click near it or selecting it in the list.
Afterwards, press *F1* on the keyboard to activate the *move mode*.
Left clicking on a location will move the selected landmark there.
When you are done, press *F1* again to deactivate the *move mode*.

#### Saving and Loading Landmarks

The placed landmarks can be saved to a JSON file.
Moreover, previously saved landmarks can also be loaded and added to the current scan.

[1]: ../dataFormats/scan.md
