#include <string>
#include <vector>

#include "settings.h"
#include "landmark/LandmarkIO.h"
#include "mesh/MeshIO.h"
#include "alignment/RigidAlignment.h"


int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  auto input = MeshIO::read(settings.source);
  auto landmarks = LandmarkIO::read(settings.landmarks);

  // only use the subset of landmarks if wanted by the user
  if(settings.subset.empty() == false) {

    std::map<std::string, Landmark> lookupMap;

    for(const Landmark& mark: landmarks) {

      lookupMap[mark.name] = mark;

    }

    landmarks.clear();

    for(const std::string name: settings.subset) {

      landmarks.push_back( lookupMap.at(name) );

    }


  }

  RigidAlignment alignment(landmarks);

  alignment.set_rotation(!settings.noRotation).\
  	    set_scaling(!settings.noScaling).\
	    set_translation(!settings.noTranslation);

  auto transformation = alignment.get_transformation();
  transformation.apply(input);
  MeshIO::write(input, settings.output);

  return 0;

}
