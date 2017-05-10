#ifndef __SCAN_H__
#define __SCAN_H__

#include "scan/ScanData.h"
#include "scan/ScanAccess.h"
#include "scan/ScanTransform.h"
#include "scan/ScanAnalysis.h"

class Scan{


public:

  /*----------------------------------------------------------------------*/

  Scan(const ScanData& data) : scanData(data) {
    this->scanAccess = new ScanAccess(this->scanData);
    this->scanTransform = new ScanTransform(this->scanData);
    this->scanAnalysis = new ScanAnalysis(this->scanData);
    this->valid = true;
  }

  /*----------------------------------------------------------------------*/

  Scan(const Scan& scan) : scanData(scan.scanData) {

    cleanup();

    this->scanAccess = new ScanAccess(this->scanData);
    this->scanTransform = new ScanTransform(this->scanData);
    this->scanAnalysis = new ScanAnalysis(this->scanData);
    this->valid = true;

  }

  /*----------------------------------------------------------------------*/

  Scan& operator=( const Scan& other ){

    cleanup();

    this->scanData = other.scanData;
    this->scanAccess = new ScanAccess(this->scanData);
    this->scanTransform = new ScanTransform(this->scanData);
    this->scanAnalysis = new ScanAnalysis(this->scanData);
    this->valid = true;

    return *this;

  }

  /*----------------------------------------------------------------------*/

  Scan() {
    this->scanAccess = nullptr;
    this->scanTransform = nullptr;
    this->scanAnalysis = nullptr;
    this->valid = false;
  }

  /*----------------------------------------------------------------------*/

  ~Scan() {
    cleanup();
  }

  /*----------------------------------------------------------------------*/

  void cleanup() {
    if( this->valid == true ) {
      delete this->scanAccess;
      delete this->scanTransform;
      delete this->scanAnalysis;
    }

    this->valid = false;

  }

  /*----------------------------------------------------------------------*/


  ScanData* data() {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return &this->scanData;
  }

  /*----------------------------------------------------------------------*/

  const ScanData* data() const {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return &this->scanData;
  }

  /*----------------------------------------------------------------------*/

  ScanAccess* access() {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return this->scanAccess;
  }

  /*----------------------------------------------------------------------*/

  const ScanAccess* access() const {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return this->scanAccess;
  }

  /*----------------------------------------------------------------------*/


  ScanTransform* transform() {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return this->scanTransform;
  }

  /*----------------------------------------------------------------------*/

  ScanAnalysis* analysis() const {

    if( this->valid == false ) {
      throw std::runtime_error("Scan data structures not initialized");
    }

    return this->scanAnalysis;
  }

  /*----------------------------------------------------------------------*/

private:

  ScanData scanData;
  ScanAccess* scanAccess;
  ScanTransform* scanTransform;
  ScanAnalysis* scanAnalysis;

  bool valid = false;

};
#endif
