#ifndef __SCAN_ANALYSIS_H__
#define __SCAN_ANALYSIS_H__

#include "scan/ScanData.h"
#include "scan/ScanAccess.h"

/* Class for analysis related operations on a scan like derivative computation */
class ScanAnalysis {

  public:

    /*----------------------------------------------------------------------*/

    ScanAnalysis(ScanData& data) :
      scanData(data),
      scanAccess(data) {
      }

    /*----------------------------------------------------------------------*/

    // compute x-derivative by using central differences
    double compute_fx(
        const int& x,
        const int& y,
        const int& z
        ) const {

      const double factor = 1. / ( 2. * this->scanData.get_hx() );

      const int left  = this->scanAccess.get_value_index_neumann(x - 1, y, z);
      const int right = this->scanAccess.get_value_index_neumann(x + 1, y, z);

      return factor * (right - left);
    }


    /*----------------------------------------------------------------------*/

    // compute y-derivative by using central differences
    double compute_fy(
        const int& x,
        const int& y,
        const int& z
        ) const {

      const double factor = 1. / ( 2. * this->scanData.get_hy() );

      const int lower = this->scanAccess.get_value_index_neumann(x, y - 1, z);
      const int upper = this->scanAccess.get_value_index_neumann(x, y + 1, z);

      return factor * (upper - lower);

    }

    /*----------------------------------------------------------------------*/

    // compute z-derivative by using central differences
    double compute_fz(
        const int& x,
        const int& y,
        const int& z
        ) const {

      const double factor = 1. / ( 2. * this->scanData.get_hz() );

      const int back  = this->scanAccess.get_value_index_neumann(x, y, z - 1);
      const int front = this->scanAccess.get_value_index_neumann(x, y, z + 1);

      return factor * (front - back);
    }

    /*----------------------------------------------------------------------*/

    arma::vec compute_gradient(
        const int& x,
        const int& y,
        const int& z
        ) const {

      arma::vec gradient = arma::zeros(3);
      gradient(0) = compute_fx(x, y, z);
      gradient(1) = compute_fy(x, y, z);
      gradient(2) = compute_fz(x, y, z);

      return gradient;
    }

    /*----------------------------------------------------------------------*/

    arma::mat compute_structure_tensor (
        const int& x,
        const int& y,
        const int& z
        ) const {

      arma::vec gradient = compute_gradient(x, y, z);

      arma::mat structureTensor(3, 3);
      structureTensor = arma::zeros(3, 3);

      structureTensor = gradient * gradient.t();

      return structureTensor;

    }

    /*----------------------------------------------------------------------*/

    /* compute region structure tensor -> derive subspace of local principal
     * gradient directions in the (2 * radius + 1)³ region centered at (x, y, z)
     */
    arma::mat compute_region_structure_tensor(
        const int& x,
        const int& y,
        const int& z,
        const int& radius
        ) const {

      arma::mat structureTensor = arma::zeros(3, 3);

      for(int i = -radius; i <= radius; ++i) {
        for(int j = -radius; j <= radius; ++j) {
          for(int k = -radius; k <= radius; ++k) {

            structureTensor += compute_structure_tensor( x + i, y + j, z + k);

          } // end for k
        } // end for j
      } // end for i

      return structureTensor / pow(2 * radius + 1, 3);

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    ScanData& scanData;
    ScanAccess scanAccess;

    /*----------------------------------------------------------------------*/

};

#endif
