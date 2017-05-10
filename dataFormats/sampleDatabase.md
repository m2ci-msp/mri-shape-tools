# Sample Database YAML Format

## Overview

The file consists of two YAML objects:

### meta

This map contains two quantities:

- **speakerAmount** : amount of speakers in the database
- **phonemeAmount** : amount of phonemes present for each speaker

### data

This object contains a list of data collections associated to the speakers.
Such a data collection is a map:

- **name** : name of the associated speaker
- **phoneme** : list of phoneme objects

A phoneme object itself is again a map:

- **prompt** : label describing the phoneme
- **path** : path to the mesh file

**Please note that for each speaker the same phonemes should be present.**

## Example

```yaml
meta: {speakerAmount: 3, phonemeAmount: 5}
data:
- name: speaker01
  phonemes:
  - {prompt: a, path: /mnt/mridata/speaker01/shape_a.ply}
  - {prompt: e, path: /mnt/mridata/speaker01/shape_e.ply}
  - {prompt: i, path: /mnt/mridata/speaker01/shape_i.ply}
  - {prompt: o, path: /mnt/mridata/speaker01/shape_o.ply}
  - {prompt: s, path: /mnt/mridata/speaker01/shape_s.ply}
- name: speaker02
  phonemes:
  - {prompt: a, path: /mnt/mridata/speaker02/shape_a.ply}
  - {prompt: e, path: /mnt/mridata/speaker02/shape_e.ply}
  - {prompt: i, path: /mnt/mridata/speaker02/shape_i.ply}
  - {prompt: o, path: /mnt/mridata/speaker02/shape_o.ply}
  - {prompt: s, path: /mnt/mridata/speaker02/shape_s.ply}
- name: speaker03
  phonemes:
  - {prompt: a, path: /mnt/mridata/speaker03/shape_a.ply}
  - {prompt: e, path: /mnt/mridata/speaker03/shape_e.ply}
  - {prompt: i, path: /mnt/mridata/speaker03/shape_i.ply}
  - {prompt: o, path: /mnt/mridata/speaker03/shape_o.ply}
  - {prompt: s, path: /mnt/mridata/speaker03/shape_s.ply}
```
