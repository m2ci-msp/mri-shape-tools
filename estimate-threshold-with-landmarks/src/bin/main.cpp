#include "image/Image.h"
#include "image/segmentation/LandmarkSegmentation.h"
#include "LandmarkReader.h"

#include "settings.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    double bestThreshold = DBL_MAX;

    for(size_t i = 0; i < settings.images.size(); ++i) {

      Image image;
      image.read().from(settings.images.at(i));

      if(settings.scaleTo8Bit == true) {

        image.values().scale(0, 255);
        image.values().round();

      }

      std::vector<arma::vec> landmarks =
        LandmarkReader::read(settings.landmarks.at(i), image);

      ImageData imageData = image.data();
      double currentThreshold =
        LandmarkSegmentation(imageData, landmarks).find_threshold();


      bestThreshold = ( currentThreshold < bestThreshold )? currentThreshold: bestThreshold;

    }

    Json::Value root;

    root["threshold"] = bestThreshold;

    std::ofstream outFile(settings.output);

    outFile << root << std::endl;

    outFile.close();

    return 0;

}
