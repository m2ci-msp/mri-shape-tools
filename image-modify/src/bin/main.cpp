#include <iostream>

#include "settings.h"
#include "image-modify/ApplyModifications.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Image image;
    image.read().from(settings.input);

    std::ifstream inFile(settings.specifications);

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();

    ApplyModifications(image).apply(buffer.str());

    image.write().to(settings.output);

    return 0;

}
