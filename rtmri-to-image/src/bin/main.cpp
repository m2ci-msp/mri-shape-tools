#include "Video.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Video video(settings.videoFile);

  video.process();

	return 0;

}
