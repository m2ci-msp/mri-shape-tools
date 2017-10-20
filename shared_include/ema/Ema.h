#ifndef __EMA_H__
  #define __EMA_H__

  #include "ema/EmaData.h"
  #include "ema/EmaAccess.h"
  #include "ema/EmaReader.h"
  #include "ema/EmaWriter.h"

  class Ema{

  public:

    // the constructor ensures that all corresponding objects store
    // a reference to the emaData member
    Ema() :
    
      emaAccess(emaData),
      emaReader(emaData),
      emaWriter(emaData) {
    
      }

    EmaAccess& access() { return this->emaAccess; }
    
    EmaReader& read() { return this->emaReader; }
    
    EmaWriter& write() { return this->emaWriter; }

  private:

    EmaData emaData;
    
    EmaAccess emaAccess;
    
    EmaReader emaReader;
    
    EmaWriter emaWriter;

  };
#endif
