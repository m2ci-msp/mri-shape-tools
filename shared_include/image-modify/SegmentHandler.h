#ifndef __SEGMENT_HANDLER_H__
#define __SEGMENT_HANDLER_H__

#include <armadillo>
#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

#include "image/segmentation/chanvese/ChanVeseSettings.h"
#include "image/segmentation/chanvese/SphereLevelSetBuilder.h"

class SegmentHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "threshold") { threshold(image, options); }
    else if (type == "otsu" ) { otsu(image); }
    else if (type == "iterated otsu" ) { iterated_otsu(image, options); }
    else if (type == "with landmarks" ) { with_landmarks(image, options); }
    else if (type == "chan vese") { chan_vese(image, options); }

  }

  void threshold(Image& image, Json::Value& options) {

    const double threshold = options["threshold"].asDouble();

    image.segment().threshold(threshold);

  }

  void otsu(Image& image) {

    image.segment().otsu();

  }

  void iterated_otsu(Image& image, Json::Value& options) {

    const int iterations = options["iterations"].asInt();

    image.segment().iterated_otsu(iterations);

  }

  void with_landmarks(Image& image, Json::Value& options) {

    std::vector<arma::vec> landmarks = build_landmarks(options);

    image.segment().with_landmarks(landmarks);

  }

  void chan_vese(Image& image, Json::Value& options) {

    // build settings
    ChanVeseSettings settings;

    settings.evolutionSteps = options["evolutionSteps"].asInt();
    settings.epsilon = options["epsilon"].asDouble();
    settings.iterationAmount = options["iterationAmount"].asInt();
    settings.timeStepSize = options["timeStepSize"].asDouble();
    settings.mu = options["mu"].asDouble();
    settings.lambdaRegionOne = options["lambdaRegionOne"].asDouble();
    settings.lambdaRegionTwo = options["lambdaRegionTwo"].asDouble();
    settings.eta = options["eta"].asDouble();
    settings.omega = options["omega"].asDouble();

    // build level set

    const arma::vec center = read_vector(options["center"]);
    const double radius = options["radius"].asDouble();

    ImageData levelSet =
      SphereLevelSetBuilder()

      .with_dimensions(
                       image.info().get_nx(),
                       image.info().get_ny(),
                       image.info().get_nz()
                      )
      .with_grid_spacings(
                          image.info().get_hx(),
                          image.info().get_hy(),
                          image.info().get_hz()
                          )
      .with_center(center)
      .with_radius(radius)
      .build();

    image.segment().chan_vese(levelSet, settings);

  }

private:


  arma::vec read_vector(const Json::Value& value) const {

    arma::vec result = arma::zeros(3);

    for(int i = 0; i < 3; ++i) {

      result(i) = value[i].asDouble();

    }

    return result;

  }

  std::vector<arma::vec> build_landmarks(Json::Value& options) {

    std::vector<arma::vec> landmarks;

    for(
        const auto& mark: options["landmarks"] ) {

      landmarks.push_back({
          mark["x"].asDouble(),
            mark["y"].asDouble(),
            mark["z"].asDouble()
            });

    }

    return landmarks;

  }

};
#endif
