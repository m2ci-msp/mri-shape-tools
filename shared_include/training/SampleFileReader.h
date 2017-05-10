#ifndef __SAMPLE_FILE_READER_H__
#define __SAMPLE_FILE_READER_H__

#include <yaml-cpp/yaml.h>

#include "mesh/MeshIO.h"

#include "SampleDataBase.h"

class SampleFileReader{

public:

  /*----------------------------------------------------------------------*/

  static SampleDataBase read_from(const std::string& fileName) {

    SampleDataBase database;

    YAML::Node training = YAML::LoadFile(fileName);

    // read meshes
    YAML::Node data = training["data"];

    for(const YAML::Node& speaker: data) {

      std::string speakerId = speaker["name"].as<std::string>();
      YAML::Node phonemes = speaker["phonemes"];

      for(const YAML::Node& phoneme: phonemes) {

        std::string phonemeId = phoneme["prompt"].as<std::string>();

        Mesh mesh = MeshIO::read(phoneme["path"].as<std::string>());

        database.add_mesh(mesh, speakerId, phonemeId);

      } // end phoneme
    } // end speaker

    return database;

  }

  /*----------------------------------------------------------------------*/

};

#endif
