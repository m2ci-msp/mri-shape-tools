#ifndef __EMA_H__
  #define __EMA_H__

  #include "ema/EmaData.h"
  #include "ema/EmaInfo.h"
  #include "ema/EmaReader.h"
  #include "ema/EmaWriter.h"
  #include "ema/EmaResample.h"

  class Ema{

  public:

    // the constructor ensures that all corresponding objects store
    // a reference to the emaData member
    Ema() :
    
      emaInfo(emaData.emaInfoData),
      emaReader(emaData),
      emaWriter(emaData),
      emaResample(emaData) {
    
      }
    
    // copy constructor -> make sure that we have no references to the original
    // object
    Ema(const Ema& other) :
    
      emaData(other.emaData),
      emaInfo(emaData.emaInfoData),
      emaReader(emaData),
      emaWriter(emaData),
      emaResample(emaData) {
    
      }

    EmaCoil& coil(const std::string& coilId) { return this->emaData.emaCoils.at(coilId); }
    
    EmaInfo& info() { return this->emaInfo; }
    
    EmaReader& read() { return this->emaReader; }
    
    EmaWriter& write() { return this->emaWriter; }
    
    EmaResample& resample() { return this->emaResample; }

  private:

    EmaData emaData;
    
    EmaInfo emaInfo;
    
    EmaReader emaReader;
    
    EmaWriter emaWriter;
    
    EmaResample emaResample;

  };
#endif
