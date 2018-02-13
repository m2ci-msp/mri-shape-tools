#ifndef __LUCAS_KANADE_ITK_WRAPPER_H__
#define __LUCAS_KANADE_ITK_WRAPPER_H__

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "energy/Transformation.h"
#include "energy/TransformationEnum.h"

namespace lucasKanade{
  /* class containing static methods from converting between ITK data structures
   * and energy data structures
   */
  class ITKWrapper{

  public:

    /*--------------------------------------------------------------------------*/

    /* converts from a vnl_vector to transformation */
    static void vnl_vector_to_transformation(
                                             const std::vector<double>& x,
                                             const arma::vec& origin,
                                             Transformation& transformation
                                             ) {

      // first read translation
      const double& tx = x[TX];
      const double& ty = x[TY];
      const double& tz = x[TZ];

      // read rotation angle around x-axis
      const double& alpha = x[ALPHA];

      // read rotation angle around x-axis
      const double& beta = x[BETA];

      // read rotation angle around x-axis
      const double& gamma = x[GAMMA];

      Translation translation(tx, ty, tz);

      transformation = Transformation(
                                      translation,
                                      alpha,
                                      beta,
                                      gamma
                                      );

      transformation.set_origin(origin);

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
