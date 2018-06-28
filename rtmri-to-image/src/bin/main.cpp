#include "Video.h"
#include "image/Image.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  PGMListReader video(settings.fileList);

  Image image = video.read_list();

  image.write().to(settings.outputFile);

	return 0;

}
