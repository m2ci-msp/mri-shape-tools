#include "image/Image.h"

#include "settings.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Image image;
    image.read().from(settings.input);

    if(settings.scaleTo8Bit == true) {

      image.values().scale(0, 255);
      image.values().round();

    }

    image.segment().threshold(settings.threshold);

    image.write().to(settings.output);

    return 0;

}
