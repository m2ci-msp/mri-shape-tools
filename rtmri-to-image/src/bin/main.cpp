#include "Video.h"
#include "image/Image.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  PGMListReader video(settings.fileList);

  video.set_spacings(settings.spacingX, settings.spacingY, settings.spacingT);

  Image image = video.create_image();

  image.write().to(settings.outputFile);

	return 0;

}
