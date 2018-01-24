#include "settings.h"
#include "image-modify/ApplyModifications.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Image image;
    image.read().from(settings.input);

    ApplyModifications(image).apply(settings.specifications);

    image.write().to(settings.output);

    return 0;

}
