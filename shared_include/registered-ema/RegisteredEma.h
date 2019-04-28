#ifndef __REGISTERED_EMA_H__
#define __REGISTERED_EMA_H__

#include "RegisteredEmaData.h"
#include "RegisteredEmaAccess.h"
#include "RegisteredEmaBoundary.h"
#include "RegisteredEmaInfo.h"
#include "RegisteredEmaMirror.h"
#include "RegisteredEmaInterpolate.h"
#include "RegisteredEmaReader.h"

class RegisteredEma{

private:

  RegisteredEmaData registeredData;
  RegisteredEmaAccess registeredAccess;
  RegisteredEmaInfo registeredInfo;
  RegisteredEmaReader registeredReader;
  RegisteredEmaInterpolate registeredInterpolate;
  RegisteredEmaBoundary registeredBoundary;
  RegisteredEmaMirror registeredMirror;

public:

  RegisteredEma() :

    registeredAccess(registeredData),
    registeredInfo(registeredData),
    registeredReader(registeredData),
    registeredInterpolate(registeredData),
    registeredBoundary(registeredData),
    registeredMirror(registeredData) {

  }

  RegisteredEma(const RegisteredEma& other) :
    registeredData(other.registeredData),
    registeredAccess(registeredData),
    registeredInfo(registeredData),
    registeredReader(registeredData),
    registeredInterpolate(registeredData),
    registeredBoundary(registeredData),
    registeredMirror(registeredData) {


  }

  // assignment operator
  RegisteredEma& operator=(const RegisteredEma& other) {

    this->registeredData = other.registeredData;
    return *this;

  }

  RegisteredEmaAccess& access() { return this->registeredAccess; }
  RegisteredEmaInfo& info() { return this->registeredInfo; }
  RegisteredEmaReader& read() { return this->registeredReader; }
  RegisteredEmaInterpolate& interpolate() { return this->registeredInterpolate; }
  RegisteredEmaBoundary& boundary() { return this->registeredBoundary; }
  RegisteredEmaMirror& mirror() { return this->registeredMirror; }

};
#endif
