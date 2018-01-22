#ifndef __SEGMENT_HANDLER_H__
#define __SEGMENT_HANDLER_H__

#include <json/json.h>

#include "singleton/LandmarkPool.h"
#include "Handler.h"
#include "image/Image.h"

class SegmentHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "threshold") { threshold(image, options); }
    else if (type == "otsu" ) { otsu(image); }
    else if (type == "cascaded otsu" ) { cascaded_otsu(image, options); }
    else if (type == "with landmarks" ) { with_landmarks(image); }

  }

  void threshold(Image& image, Json::Value& options) {

    const double threshold = options["threshold"].asDouble();

    image.segment().threshold(threshold);

  }

  void otsu(Image& image) {

    image.segment().otsu();

  }

  void cascaded_otsu(Image& image, Json::Value& options) {

    const int iterations = options["iterations"].asInt();

    image.segment().cascaded_otsu(iterations);

  }

  void with_landmarks(Image& image) {

    std::vector<arma::vec> landmarks = build_landmarks();

    image.segment().with_landmarks(landmarks);

  }

private:

  std::vector<arma::vec> build_landmarks() {

    std::vector<arma::vec> landmarks;

    for(
        const auto& mark: LandmarkPool::get_instance()->get_all_landmarks()) {

      landmarks.push_back({
          mark->get_position().get_canonical_x(),
          mark->get_position().get_canonical_y(),
          mark->get_position().get_canonical_z()
            });

    }

    return landmarks;

  }

};
#endif
