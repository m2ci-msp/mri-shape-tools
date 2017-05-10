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
      const double& s = 1;

      // read rotation angle
      const double& theta = x[THETA];

      // read rotation axis
      const double& ax = x[AX];
      const double& ay = x[AY];
      const double& az = x[AZ];

      Translation translation(tx, ty, tz);
      Scaling scaling(s);
      Rotation rotation(ax, ay, az, theta);

      transformation = Transformation(
          rotation,
          scaling,
          translation
          );

      transformation.set_origin(mesh.get_center());

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
