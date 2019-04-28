#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "ImageAccess.h"
#include "ImageNeumannAccess.h"
#include "ImageArithmetic.h"
#include "ImageCalculus.h"
#include "ImageCrop.h"
#include "ImageData.h"
#include "ImageBoundary.h"
#include "ImageCoordinate.h"
#include "ImageCreate.h"
#include "ImageExtrema.h"
#include "ImageFilter.h"
#include "ImageHistogram.h"
#include "ImageInfo.h"
#include "ImageInterpolate.h"
#include "ImageMirror.h"
#include "ImageMorphology.h"
#include "ImageReader.h"
#include "ImageSegmentation.h"
#include "ImageValues.h"
#include "ImageWriter.h"
#include "ImageStatistics.h"
#include "ImageEdge.h"
#include "ImageInterpolateSlice.h"

class Image{

public:

  // default constructor
  Image() :
    imageAccess(imageData),
    imageNeumannAccess(imageData),
    imageArithmetic(imageData),
    imageBoundary(imageData),
    imageCalculus(imageData),
    imageCoordinate(imageData),
    imageCrop(imageData),
    imageCreate(imageData),
    imageExtrema(imageData),
    imageFilter(imageData),
    imageHistogram(imageData),
    imageInfo(imageData),
    imageInterpolate(imageData),
    imageMirror(imageData),
    imageMorphology(imageData),
    imageReader(imageData),
    imageSegmentation(imageData),
    imageValues(imageData),
    imageWriter(imageData),
    imageStatistics(imageData),
    imageEdge(imageData),
    imageInterpolateSlice(imageData) {
  }

  // copy constructor
  Image(const Image& other) :
    imageData(other.imageData),
    imageAccess(imageData),
    imageNeumannAccess(imageData),
    imageArithmetic(imageData),
    imageBoundary(imageData),
    imageCalculus(imageData),
    imageCoordinate(imageData),
    imageCrop(imageData),
    imageCreate(imageData),
    imageExtrema(imageData),
    imageFilter(imageData),
    imageHistogram(imageData),
    imageInfo(imageData),
    imageInterpolate(imageData),
    imageMirror(imageData),
    imageMorphology(imageData),
    imageReader(imageData),
    imageSegmentation(imageData),
    imageValues(imageData),
    imageWriter(imageData),
    imageStatistics(imageData),
    imageEdge(imageData),
    imageInterpolateSlice(imageData) {
  }

  // assignment operator
  Image& operator=(const Image& other) {

    // copy the data of the other image
    this->imageData = other.imageData;
    return *this;

  }

  // reference getters for all members except for data
  ImageAccess& access() { return this->imageAccess; }
  ImageNeumannAccess& neumann_access() { return this->imageNeumannAccess; }
  ImageArithmetic& arithmetic() { return this->imageArithmetic; }
  ImageBoundary& boundary() { return this->imageBoundary; }
  ImageCalculus& calculus() { return this->imageCalculus; }
  ImageCoordinate& coordinate() { return this->imageCoordinate; }
  ImageCrop& crop() { return this->imageCrop; }
  ImageCreate& create() { return this->imageCreate; }
  ImageExtrema& extrema() { return this->imageExtrema; }
  ImageFilter& filter() { return this->imageFilter; }
  ImageHistogram& histogram() { return this->imageHistogram; }
  const ImageInfo& info() const { return this->imageInfo; }
  ImageInterpolate& interpolate() { return this->imageInterpolate; }
  ImageMirror& mirror() { return this->imageMirror; }
  ImageMorphology& morphology() { return this->imageMorphology; }
  ImageReader& read() { return this->imageReader; }
  ImageSegmentation& segment() { return this->imageSegmentation; }
  ImageValues& values() { return this->imageValues; }
  ImageWriter& write() { return this->imageWriter; }
  ImageStatistics& statistics() { return this->imageStatistics; }
  ImageEdge& edge() { return this->imageEdge; }
  ImageInterpolateSlice& interpolate_slice() { return this->imageInterpolateSlice; }

  // get copy of data
  ImageData data() const { return this->imageData; }

private:

  ImageData imageData;
  ImageAccess imageAccess;
  ImageNeumannAccess imageNeumannAccess;
  ImageArithmetic imageArithmetic;
  ImageBoundary imageBoundary;
  ImageCalculus imageCalculus;
  ImageCoordinate imageCoordinate;
  ImageCrop imageCrop;
  ImageCreate imageCreate;
  ImageExtrema imageExtrema;
  ImageFilter imageFilter;
  ImageHistogram imageHistogram;
  ImageInfo imageInfo;
  ImageInterpolate imageInterpolate;
  ImageMirror imageMirror;
  ImageMorphology imageMorphology;
  ImageReader imageReader;
  ImageSegmentation imageSegmentation;
  ImageValues imageValues;
  ImageWriter imageWriter;
  ImageStatistics imageStatistics;
  ImageEdge imageEdge;
  ImageInterpolateSlice imageInterpolateSlice;

};

#endif
