#ifndef __FIT_MODEL_ITK_WRAPPER_H__
#define __FIT_MODEL_ITK_WRAPPER_H__

#include <armadillo>
#include <vnl/vnl_vector.h>

namespace fitModel{
  /* class containing static methods from converting between ITK data structures
   * and energy data structures
   */
  class ITKWrapper{

  public:

    /*--------------------------------------------------------------------------*/

    /* converts from a vnl_vector to model weights */
    static void vnl_vector_to_weights(
      const vnl_vector<double>& x,
      arma::vec& speakerWeights,
      arma::vec& phonemeWeights
      ) {

      for(unsigned int i = 0; i < speakerWeights.size(); ++i) {
        speakerWeights(i) = x[i];
      }

      for(unsigned int i = 0; i < phonemeWeights.size(); ++i) {
        phonemeWeights(i) = x[speakerWeights.size() + i];
      }

    }

    /*--------------------------------------------------------------------------*/

    /* converts from model weights to a vnl_vector */
    static void weights_to_vnl_vector(
      const arma::vec& speakerWeights,
      const arma::vec& phonemeWeights,
      vnl_vector<double>& x
      ) {

      for(unsigned int i = 0; i < speakerWeights.size(); ++i) {
        x[i] = speakerWeights(i);
      }

      for(unsigned int i = 0; i < phonemeWeights.size(); ++i) {
        x[speakerWeights.size() + i] = phonemeWeights(i);
      }

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
