#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"

#include <string>
#include <fstream>

#include <json/json.h>

class Settings {

public:

  // input and output
  std::string model;

  std::string input;

  std::string output;

  std::vector<std::string> channels;
  std::vector<int> sourceIds;

  std::string reference;

  double scale = 1;
  std::string unit = std::string("mm");

  Settings(int argc, char* argv[]) {

    std::string coilConfig;

    // input and output
    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> coilConfigFlag("coilConfig", coilConfig);
    FlagSingle<std::string> outputFlag("output", this->output);

    // unit
    FlagSingle<std::string> unitFlag("unit", this->unit, true);

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&inputFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&outputFlag);

    // channel and vertex id correspondences
    parser.define_flag(&coilConfigFlag);

    // unit
    parser.define_flag(&unitFlag);

    // Parse the command line
    parser.parse_from_command_line(argc, argv);

    // read coil configurarion
    read_coil_config(coilConfig);
    
    // Define the scale according to the unit
    this->scale = derive_scale_from(this->unit);

  }

  /*---------------------------------------------------------------------------*/

private:

  /*---------------------------------------------------------------------------*/

  double derive_scale_from(const std::string& unit) {

    if( unit == "mm" ) {
      return 1.;
    }
    else if ( unit == "cm" ) {
      return 0.1;
    }
    else if ( unit == "dm" ) {
      return 0.01;
    }
    else if ( unit == "m" ) {
      return 0.001;
    }
    else {
      throw std::runtime_error("Unit " + unit + " is unknown.");
    }
    
  }

  /*---------------------------------------------------------------------------*/

  void read_coil_config(const std::string& fileName) {

    // try to open file
    std::ifstream inFile(fileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open configuration file.");
    }

    Json::Value root;
    // read frame data
    Json::Reader reader;
    reader.parse(inFile, root); 

    this->reference = root["referenceName"].asString();

    for(const Json::Value& value: root["coils"] ) {
      
      this->channels.push_back(value["coilName"].asString());
      this->sourceIds.push_back(value["vertexId"].asInt());

    }

  }

  /*---------------------------------------------------------------------------*/

};

#endif
