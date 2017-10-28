#ifndef __EMA_COIL_H__
#define __EMA_COIL_H__

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilInfo.h"
#include "ema/EmaCoilAccess.h"
#include "ema/EmaCoilInterpolate.h"
#include "ema/EmaCoilTransform.h"
#include "ema/EmaCoilBoundary.h"
#include "ema/EmaCoilMirror.h"
#include "ema/EmaCoilCalculus.h"
#include "ema/EmaCoilArithmetic.h"
#include "ema/EmaCoilFilter.h"

class EmaCoil{

public:

  EmaCoil(const EmaCoilData& emaCoilDataReference) : 
  
    emaCoilData(emaCoilDataReference),
    emaCoilInfo(emaCoilData),
    emaCoilAccess(emaCoilData),
    emaCoilInterpolate(emaCoilData),
    emaCoilTransform(emaCoilData),
    emaCoilBoundary(emaCoilData),
    emaCoilMirror(emaCoilData),
    emaCoilCalculus(emaCoilData),
    emaCoilArithmetic(emaCoilData),
    emaCoilFilter(emaCoilData) {
  
  }
  
  // copy constructor -> make sure that the members of the copy no longer
  // reference the emaCoilData object of the original object
  EmaCoil(const EmaCoil& other) :
  
    emaCoilData(other.emaCoilData),
    emaCoilInfo(emaCoilData),
    emaCoilAccess(emaCoilData),
    emaCoilInterpolate(emaCoilData),
    emaCoilTransform(emaCoilData),
    emaCoilBoundary(emaCoilData),
    emaCoilMirror(emaCoilData),
    emaCoilCalculus(emaCoilData),
    emaCoilArithmetic(emaCoilData),
    emaCoilFilter(emaCoilData) {
  
    }

  const EmaCoilInfo& info() const { return this->emaCoilInfo; }
  
  EmaCoilAccess& access() { return this->emaCoilAccess; }
  
  const EmaCoilAccess& access() const { return this->emaCoilAccess; }
  
  EmaCoilInterpolate& interpolate() { return this->emaCoilInterpolate; }
  
  const EmaCoilInterpolate& interpolate() const { return this->emaCoilInterpolate; }
  
  EmaCoilTransform& transform() { return this->emaCoilTransform; }
  
  const EmaCoilTransform& transform() const { return this->emaCoilTransform; }
  
  EmaCoilBoundary& boundary() { return this->emaCoilBoundary; }
  
  const EmaCoilBoundary& boundary() const { return this->emaCoilBoundary; }
  
  EmaCoilMirror& mirror() { return this->emaCoilMirror; }
  
  const EmaCoilMirror& mirror() const { return this->emaCoilMirror; }
  
  EmaCoilCalculus& calculus() { return this->emaCoilCalculus; }
  
  const EmaCoilCalculus& calculus() const { return this->emaCoilCalculus; }
  
  EmaCoilArithmetic& arithmetic() { return this->emaCoilArithmetic; }
  
  const EmaCoilArithmetic& arithmetic() const { return this->emaCoilArithmetic; }
  
  EmaCoilFilter& filter() { return this->emaCoilFilter; }
  
  const EmaCoilFilter& filter() const { return this->emaCoilFilter; }

private:

  EmaCoilData emaCoilData;
  
  EmaCoilInfo emaCoilInfo;
  
  EmaCoilAccess emaCoilAccess;
  
  EmaCoilInterpolate emaCoilInterpolate;
  
  EmaCoilTransform emaCoilTransform;
  
  EmaCoilBoundary emaCoilBoundary;
  
  EmaCoilMirror emaCoilMirror;
  
  EmaCoilCalculus emaCoilCalculus;
  
  EmaCoilArithmetic emaCoilArithmetic;
  
  EmaCoilFilter emaCoilFilter;

};
#endif
