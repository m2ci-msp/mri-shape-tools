#ifndef __RIGID_ALIGNMENT_ITK_WRAPPER_H__
#define __RIGID_ALIGNMENT_ITK_WRAPPER_H__

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "mesh/Mesh.h"

#include "optimization/rigidalignment/Transformation.h"
#include "optimization/rigidalignment/TransformationEnum.h"

namespace rigidAlignment{
  /* class containing static methods from converting between ITK data structures
   * and energy data structures
   */
  class ITKWrapper{

  public:

    /*--------------------------------------------------------------------------*/

    /* converts from a vnl_vector to transformation */
    static void vnl_vector_to_transformation(
                                             const vnl_vector<double>& x,
                                             Transformation& transformation,
                                             const Mesh& mesh
                                             ) {

      // first read translation
      const double& tx = x[TX];
      const double& ty = x[TY];
      const double& tz = x[TZ];

      // read scaling
      const double& s = x[S];

      // read rotation angles
      const double& alpha = x[ALPHA];
      const double& beta = x[BETA];
      const double& gamma = x[GAMMA];

      Translation translation(tx, ty, tz);
      Scaling scaling(s);

      transformation = Transformation(
                                      translation,
                                      scaling,
                                      alpha,
                                      beta,
                                      gamma
                                      );

      transformation.set_origin(mesh.get_center());

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
