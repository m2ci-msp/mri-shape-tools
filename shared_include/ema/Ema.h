#ifndef __EMA_H__
#define __EMA_H__

#include "ema/EmaData.h"
#include "ema/EmaInfo.h"
#include "ema/EmaReader.h"
#include "ema/EmaWriter.h"
#include "ema/EmaResample.h"
#include "ema/EmaHeadCorrection.h"
#include "ema/EmaTransform.h"
#include "ema/EmaCoilSubset.h"
#include "ema/EmaPointCloud.h"


class Ema{

private:

  EmaData emaData;
  EmaInfo emaInfo;
  EmaReader emaReader;
  EmaWriter emaWriter;
  EmaResample emaResample;
  EmaHeadCorrection emaHeadCorrection;
  EmaTransform emaTransform;
  EmaCoilSubset emaCoilSubset;
  EmaPointCloud emaPointCloud;

public:

  // the constructor ensures that all corresponding objects store
  // a reference to the emaData member
  Ema() :
    
    emaInfo(emaData.emaInfoData),
    emaReader(emaData),
    emaWriter(emaData),
    emaResample(emaData),
    emaHeadCorrection(emaData),
    emaTransform(emaData),
    emaCoilSubset(emaData),
    emaPointCloud(emaData) {

  }
    
  // copy constructor -> make sure that we have no references to the original
  // object
  Ema(const Ema& other) :
    
    emaData(other.emaData),
    emaInfo(emaData.emaInfoData),
    emaReader(emaData),
    emaWriter(emaData),
    emaResample(emaData),
    emaHeadCorrection(emaData),
    emaTransform(emaData),
    emaCoilSubset(emaData),
    emaPointCloud(emaData) {
    
  }

  // assignment operator
  Ema& operator=(const Ema& other) {

    this->emaData = other.emaData;
    return *this;

  }

  EmaCoil& coil(const std::string& coilId) { return this->emaData.emaCoils.at(coilId); }
    
  EmaInfo& info() { return this->emaInfo; }
    
  EmaReader& read() { return this->emaReader; }

  EmaWriter& write() { return this->emaWriter; }

  EmaResample& resample() { return this->emaResample; }

  EmaHeadCorrection& head_correct() { return this->emaHeadCorrection; }

  EmaTransform& transform_all_coils() { return this->emaTransform; }

  EmaCoilSubset& reduce_coil_set() { return this->emaCoilSubset; }

  EmaPointCloud& point_cloud() { return this->emaPointCloud; }

};
#endif
