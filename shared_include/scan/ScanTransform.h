#ifndef __SCAN_TRANSFORM_H__
#define __SCAN_TRANSFORM_H__

#include <cfloat>
#include <set>

#include "scan/ScanData.h"
#include "scan/ScanAccess.h"

/* Class for applying transforms to a scan */

class ScanTransform {

public:

  /*----------------------------------------------------------------------*/

  ScanTransform(ScanData& data) :
    scanAccess(data),
    values(data.get_values()),
    orgX(data.get_org_x()),
    orgY(data.get_org_y()),
    orgZ(data.get_org_z()),
    nx(data.get_nx()),
    ny(data.get_ny()),
    nz(data.get_nz()),
    hx(data.get_hx()),
    hy(data.get_hy()),
    hz(data.get_hz()),
    min(data.get_min()),
    max(data.get_max()) {
  }

  /*----------------------------------------------------------------------*/


  void normalize() {
    // scale values to [0, 1]
    scale_values(0, 1);
  }

  /*----------------------------------------------------------------------*/

  void crop(
    const double& minX,
    const double& minY,
    const double& minZ,
    const double& maxX,
    const double& maxY,
    const double& maxZ
    ) {

    // sanity checks
    if( minX < 0 ) { throw std::runtime_error("minX < 0"); }
    if( minY < 0 ) { throw std::runtime_error("minY < 0"); }
    if( minZ < 0 ) { throw std::runtime_error("minZ < 0"); }

    if( maxX > nx ) { throw std::runtime_error("maxX > nx"); }
    if( maxY > ny ) { throw std::runtime_error("maxY > ny"); }
    if( maxZ > nz ) { throw std::runtime_error("maxZ > nz"); }

    if( minX > maxX ) { throw std::runtime_error("minX > maxX"); }
    if( minY > maxY ) { throw std::runtime_error("minY > maxY"); }
    if( minZ > maxZ ) { throw std::runtime_error("minZ > maxZ"); }

    std::vector<double> croppedScan;

    double newMax = -DBL_MAX;
    double newMin = DBL_MAX;

    // create cropped version
    for(int k = minZ; k <= maxZ; ++k) {
      for(int i = minX; i <= maxX; ++i) {
        for(int j = minY; j <= maxY; ++j) {
          const double value = this->scanAccess.get_value_index(i, j, k);
          croppedScan.push_back(value);
          newMax = ( value > newMax )? value: newMax;
          newMin = ( value < newMin )? value: newMin;
        }
      }
    }

    // set values
    this->values = croppedScan;

    // set extrema
    this->min = newMin;
    this->max = newMax;

    // update dimensions, consider also boundary points
    this->nx = maxX - minX + 1;
    this->ny = maxY - minY + 1;
    this->nz = maxZ - minZ + 1;

    // update origin
    this->orgX = this->orgX + minX;
    this->orgY = this->orgY + minY;
    this->orgZ = this->orgZ + minZ;

  }

  /*----------------------------------------------------------------------*/

  /* linearly scales values to the interval [minNew, maxNew] */
  void scale_values(const double& minNew, const double& maxNew) {

    if( minNew > maxNew ) {
      throw std::runtime_error("min > max");
    }

    const double dist = this->max - this->min;
    const double distNew = maxNew - minNew;

    for(auto& value: this->values) {
      // shift to zero
      value -= this->min;

      // rescale
      value *= distNew / dist;

      // shift to minNew
      value += minNew;
    }

    // update minimum and maximum
    this->min = minNew;
    this->max = maxNew;
  }

  /*----------------------------------------------------------------------*/

  /* scale the spacings by a given factor */
  void scale_spacings(const double& factor) {

    this->hx *= factor;
    this->hy *= factor;
    this->hz *= factor;

  }

  /*----------------------------------------------------------------------*/

  // rounds all values down
  void round() {

    for(double& value: this->values) {
      value = (int) value;
    }

    this->min = (int) this->min;
    this->max = (int) this->max;

  }

  /*----------------------------------------------------------------------*/

  void threshold(const double& t) {

    for(double& value: this->values) {
      if(value < t) {
        this->min = 0;
        value = 0;
      }
      else {
        value = 255;
        this->max = 255;
      }
    }

  }

  /*----------------------------------------------------------------------*/

  /* Discards colors at the lower and upper end of the color range.
     Combined with the scale_values method, it reproduces the method
     described on

     http://www.imagemagick.org/Usage/color_mods/#normalize

     to enhance the contrast.
  */
  void discard_values(
    // percentage of darkest colors to discard
    const double& lower,
    // percentage of brightest colors to discard
    const double& upper
    ) {

    // order values
    std::multiset<double> sorted;
    for(const double& value: this->values) {
      sorted.insert(value);
    }

    // get indices
    int lowerIndex = this->values.size() / 100. * lower;
    int upperIndex = this->values.size() / 100. * (100 - upper);

    // create vector for accessing the values
    std::vector<double> sortedVector(sorted.begin(), sorted.end());

    // get new minimum and maximum
    const double newMin = sortedVector.at(lowerIndex);
    const double newMax = sortedVector.at(upperIndex);

    // discard colors
    for(double& value: this->values) {
      value = (value < newMin )? newMin : value;
      value = (value > newMax )? newMax : value;
    }

    // set extrema
    this->min = newMin;
    this->max = newMax;

  }

  /*----------------------------------------------------------------------*/

private:

  ScanAccess scanAccess;

  std::vector<double>& values;

  // origin
  double& orgX;
  double& orgY;
  double& orgZ;

  // dimensions
  int& nx;
  int& ny;
  int& nz;

  // spacings
  double& hx;
  double& hy;
  double& hz;

  // extrema
  double& min;
  double& max;
};
#endif
