#ifndef __CROSS_PRODUCT_PART_H__
#define __CROSS_PRODUCT_PART_H__

#include <armadillo>

// TODO: Rename class to ProjectionPart
class CrossProductPart{

public:

  /*--------------------------------------------------------------------------*/

  CrossProductPart() {
  }

  /*--------------------------------------------------------------------------*/

  CrossProductPart(
    const double& ax,
    const double& ay,
    const double& az
    ) {


    this->A = arma::mat{

      { pow(ax, 2), ax * ay   , ax * az    },
      { ax * ay   , pow(ay, 2), ay * az    },
      { ax * az   , ay * az   , pow(az, 2) }

    };

    this->A_ax = arma::mat{

      { 2 * ax, ay, az },
      { ay    , 0 , 0  },
      { az    , 0 , 0  }

    };

    this->A_ay = arma::mat{

      { 0 , ay    , 0 },
      { ax, 2 * ay, az },
      { 0 , az    , 0 }

    };

    this->A_az = arma::mat{

      { 0 , 0 , ax     },
      { 0 , 0 , ay     },
      { ax, ay, 2 * az }

    };

  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_matrix() const { return A; }
  const arma::mat& get_derivative_ax() const { return A_ax; }
  const arma::mat& get_derivative_ay() const { return A_ay; }
  const arma::mat& get_derivative_az() const { return A_az; }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  arma::mat A;
  arma::mat A_ax;
  arma::mat A_ay;
  arma::mat A_az;

  /*--------------------------------------------------------------------------*/

};

#endif
