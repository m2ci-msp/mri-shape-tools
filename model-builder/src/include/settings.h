#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string samples;
  std::string output;
  std::string jsonOutput;
  std::string outputMeanMeshFile;
  std::string outputPhonemeWeightsFile;

  int truncatedSpeakerDimension = 0;
  int truncatedPhonemeDimension = 0;

  bool truncateSpeaker = false;
  bool truncatePhoneme = false;

  bool outputMeanMesh = false;
  bool outputPhonemeWeights = false;

  bool outputJson = false;

  Settings(int argc, char* argv[]) {


    FlagSingle<std::string> samplesFlag("samples", this->samples);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> jsonOutputFlag("jsonOutput", this->jsonOutput, true);
    FlagSingle<std::string> outputMeanMeshFlag("outputMesh",
                                               this->outputMeanMeshFile,
                                               true);
    FlagSingle<std::string> outputPhonemeWeightsFlag("outputPhonemeWeights",
                                               this->outputPhonemeWeightsFile,
                                               true);

    FlagSingle<int> truncatedSpeakerFlag("truncatedSpeaker",
                                         this->truncatedSpeakerDimension,
                                         true);
    FlagSingle<int> truncatedPhonemeFlag("truncatedPhoneme",
                                         this->truncatedPhonemeDimension,
                                         true);

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&samplesFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&jsonOutputFlag);
    parser.define_flag(&outputMeanMeshFlag);
    parser.define_flag(&outputPhonemeWeightsFlag);

    parser.define_flag(&truncatedSpeakerFlag);
    parser.define_flag(&truncatedPhonemeFlag);

    parser.parse_from_command_line(argc, argv);

    this->truncateSpeaker = truncatedSpeakerFlag.is_present();
    this->truncatePhoneme = truncatedPhonemeFlag.is_present();
    this->outputMeanMesh = outputMeanMeshFlag.is_present();
    this->outputPhonemeWeights = outputPhonemeWeightsFlag.is_present();
    this->outputJson = jsonOutputFlag.is_present();

  }

};

#endif
