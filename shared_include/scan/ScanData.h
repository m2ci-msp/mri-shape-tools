#ifndef __SCAN_DATA_H__
#define __SCAN_DATA_H__

#include <vector>
#include <cfloat>

/* Class for setting and getting basic data of a scan */
class ScanData {

  public:
    ScanData() {
    }

    /*----------------------------------------------------------------------*/
    /* member setters */
    /*----------------------------------------------------------------------*/

    ScanData& set_dimensions(
      const int& nx,
      const int& ny,
      const int& nz) {

      this->nx = nx;
      this->ny = ny;
      this->nz = nz;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    ScanData& set_spacings(
      const double& hx,
      const double& hy,
      const double& hz) {

      this->hx = hx;
      this->hy = hy;
      this->hz = hz;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    ScanData& set_extrema(
      const double& min,
      const double& max) {

      this->min = min;
      this->max = max;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    ScanData& set_data(const std::vector<double>& values) {

      this->values = values;

      update_extrema();

      return *this;
    }

    /*----------------------------------------------------------------------*/

    ScanData& set_origin(
        const double& orgX,
        const double& orgY,
        const double& orgZ) {

      this->orgX = orgX;
      this->orgY = orgY;
      this->orgZ = orgZ;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    /*----------------------------------------------------------------------*/
    /* member getters */
    /*----------------------------------------------------------------------*/

    // const versions
    const std::vector<double>& get_values() const { return this->values; }
    const int& get_nx() const { return this->nx; }
    const int& get_ny() const { return this->ny; }
    const int& get_nz() const { return this->nz; }

    const double& get_hx() const { return this->hx; }
    const double& get_hy() const { return this->hy; }
    const double& get_hz() const { return this->hz; }

    const double& get_min() const { return this->min; }
    const double& get_max() const { return this->max; }

    const double& get_org_x() const { return this->orgX; }
    const double& get_org_y() const { return this->orgY; }
    const double& get_org_z() const { return this->orgZ; }

    std::vector<double>& get_values() { return this->values; }
    int& get_nx() { return this->nx; }
    int& get_ny() { return this->ny; }
    int& get_nz() { return this->nz; }

    double& get_hx() { return this->hx; }
    double& get_hy() { return this->hy; }
    double& get_hz() { return this->hz; }

    double& get_min() { return this->min; }
    double& get_max() { return this->max; }

    double& get_org_x() { return this->orgX; }
    double& get_org_y() { return this->orgY; }
    double& get_org_z() { return this->orgZ; }

    /*----------------------------------------------------------------------*/


  private:

    /*----------------------------------------------------------------------*/

  void update_extrema() {

    this->min = DBL_MAX;
    this->max = - DBL_MAX;

    for( const double& value : this->values ) {

      this->min = ( value < this->min )? value : this->min;
      this->max = ( value > this->max )? value : this->max;

    }

  }

    /*----------------------------------------------------------------------*/

    // MR values
    std::vector<double> values;

    // origin
    double orgX;
    double orgY;
    double orgZ;

    // dimensions
    int nx;
    int ny;
    int nz;

    // spacings
    double hx;
    double hy;
    double hz;

    // extrema
    double min;
    double max;

    /*----------------------------------------------------------------------*/

};

#endif
