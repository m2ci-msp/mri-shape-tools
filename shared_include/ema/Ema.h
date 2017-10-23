#ifndef __EMA_H__
#define __EMA_H__

#include "ema/EmaData.h"
#include "ema/EmaAccess.h"
#include "ema/EmaReader.h"
#include "ema/EmaWriter.h"
#include "ema/EmaTransform.h"
#include "ema/EmaInterpolate.h"
#include "ema/EmaResample.h"

class Ema{

 public:

  // the constructor ensures that all corresponding objects store
  // a reference to the emaData member
 Ema() :
    
  emaAccess(emaData),
    emaReader(emaData),
    emaWriter(emaData),
    emaTransform(emaData),
    emaInterpolate(emaData),
    emaResample(emaData) {
    
    }

  EmaAccess& access() { return this->emaAccess; }
    
  EmaReader& read() { return this->emaReader; }
    
  EmaWriter& write() { return this->emaWriter; }

  EmaTransform& transform() { return this->emaTransform; }

  EmaInterpolate& interpolate() { return this->emaInterpolate; }

  EmaResample& resample() { return this->emaResample; }

 private:

  EmaData emaData;
    
  EmaAccess emaAccess;
    
  EmaReader emaReader;
    
  EmaWriter emaWriter;

  EmaTransform emaTransform;

  EmaInterpolate emaInterpolate;

  EmaResample emaResample;

};
#endif
