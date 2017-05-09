#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>
#include <vector>

#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"

class Output{

public:

  /*--------------------------------------------------------------------------*/

  Output(
    const std::vector<Mesh>& outputMeshes,
    const std::vector<std::string>& outputNames
    ):
    outputMeshes(outputMeshes),
    outputNames(outputNames) {
  }

  /*--------------------------------------------------------------------------*/

  void write() {
    if(this->outputMeshes.size() != outputNames.size()) {
      write_to_generated_names();
    }
    else{
      write_to_provided_names();
    }
  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void write_to_provided_names() {

    for(unsigned int i = 0; i < this->outputMeshes.size(); ++i) {
      MeshIO::write(this->outputMeshes.at(i), this->outputNames.at(i));
    }

  }

  /*--------------------------------------------------------------------------*/

  void write_to_generated_names() {

    const int maxLength = std::log10(this->outputMeshes.size()) + 1;

    for(unsigned int i = 1; i <= this->outputMeshes.size(); ++i) {

      // use first output name as base name
      std::string outputName =
        this->outputNames.at(0) +
        // pad with zeros
        std::string(
          maxLength -
          static_cast<int>(std::log10(i)) - 1, '0') +
        std::to_string(i) + ".ply";

      MeshIO::write(this->outputMeshes.at(i-1), outputName);
    }

  }

  /*--------------------------------------------------------------------------*/

  const std::vector<Mesh>& outputMeshes;
  const std::vector<std::string>& outputNames;

};
#endif
