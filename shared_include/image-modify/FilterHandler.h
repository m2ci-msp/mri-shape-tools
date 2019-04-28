#ifndef __FILTER_HANDLER_H__
#define __FILTER_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class FilterHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "gaussian") { gaussian(image, options); }
    else if (type == "diffusion" ) { diffusion(image, options); }
    else if( type == "median" ) { median(image, options); }

  }

  void gaussian(Image& image, Json::Value& options) {

    const double sigma = options["sigma"].asDouble();
    image.filter().gaussian(sigma);

  }

  void diffusion(Image& image, Json::Value& options) {

    const int timeSteps = options["timeSteps"].asInt();
    double stepSize = 0;

    if( options["stepSize"] == "automatic" ) {

      const double& hx = image.info().get_hx();
      const double& hy = image.info().get_hy();
      const double& hz = image.info().get_hz();

      stepSize = 0.5 / ( pow(hx, -2) + pow(hy, -2) + pow(hz, -2) );

    }
    else {

      stepSize = options["stepSize"].asDouble();

    }

    const double contrastLambda = options["contrastLambda"].asDouble();
    const double integrationRho = options["integrationRho"].asDouble();
    const double presmoothSigma = options["presmoothSigma"].asDouble();

    if( options.isMember("spacings") == true ) {

      Json::Value spacings = options["spacings"];
      const double hx = spacings[0].asDouble();
      const double hy = spacings[1].asDouble();
      const double hz = spacings[2].asDouble();

      // update step size if needed
      if( options["stepSize"] == "automatic" ) {

        stepSize = 0.5 / ( pow(hx, -2) + pow(hy, -2) + pow(hz, -2) );

      }

      image.filter().diffusion(timeSteps, stepSize, contrastLambda, integrationRho, presmoothSigma, hx, hy, hz);

    }
    else {

      image.filter().diffusion(timeSteps, stepSize, contrastLambda, integrationRho, presmoothSigma);

    }

  }

  void median(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.filter().median(radius);

  }

};
#endif
