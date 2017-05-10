#ifndef __MODEL_IO_H__
#define __MODEL_IO_H__

#include <string>
#include <fstream>

#include <json/json.h>
#include <armadillo>

#include "model/reader/RestrictedModelReader.h"
//#include "model/reader/ModelReader.h"

class ModelIO{

public:

  static Model read_from(const std::string& fileName) {

      std::regex regEx("[.]([[:alpha:]]+)$");
      std::smatch match;
      std::string extension;

      std::regex_search(fileName, match, regEx);

      extension = match[1];

      if( extension == "rmm" ) {
        RestrictedModelReader reader;
        return reader.read_from(fileName);
      }
/*      else if( extension == "mm" ) {
        ModelReader reader;
        return reader.read_from(file);
      }
*/
      else {
        throw std::runtime_error("Model format " + extension + " not supported.");
      }

  }

};

#endif
