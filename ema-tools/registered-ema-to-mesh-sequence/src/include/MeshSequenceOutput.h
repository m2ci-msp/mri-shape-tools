#ifndef __MESH_SEQUENCE_OUTPUT_H__
#define __MESH_SEQUENCE_OUTPUT_H__

#include <utility>
#include <vector>

#include <sstream>
#include <iomanip>

#include <fstream>
#include <json/json.h>

#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"

class MeshSequenceOutput{

private:

  const std::vector< std::pair<double, Mesh> > sequence;

  const std::string outputBaseName;

  int indexWidth;

public:

  MeshSequenceOutput(
                     const std::vector< std::pair<double, Mesh> >& sequence,
                     const std::string& outputBaseName ) :

    sequence(sequence),
    outputBaseName(outputBaseName) {

    compute_needed_index_width();

  }

  void output() {

    output_meta_file();

    for(size_t i = 0; i < this->sequence.size(); ++i) {

      output_mesh(i);

    }

  }

private:

  void output_meta_file() {

    Json::Value metaData = build_meta_data();

    std::string outputFile = this->outputBaseName + "_meta.json";

    std::ofstream output(outputFile);

    output << metaData << std::endl;

    output.close();

  }

  Json::Value build_meta_data() {

    Json::Value meshList(Json::arrayValue);

    for(size_t i = 0; i < this->sequence.size(); ++i) {

      Json::Value entry(Json::objectValue);

      entry["mesh"] = generate_output_name(i);
      entry["timeStamp"] = this->sequence.at(i).first;
      meshList.append(entry);

    }

    return meshList;

  }

  void output_mesh(const int& index) {

    std::string outputName = generate_output_name(index);

    const Mesh& mesh = this->sequence.at(index).second;

    MeshIO::write(mesh, outputName);

  }

  void compute_needed_index_width() {

    // use a string stream to compute string length for largest index
    std::stringstream largestIndex;

    largestIndex << this->sequence.size() - 1;

    this->indexWidth = largestIndex.str().size();

  }

  std::string generate_output_name(const int& index) {

    std::stringstream output;

    std::ios_base::fmtflags original = output.flags();

    output << this->outputBaseName
           << "_mesh"
           << std::setfill('0')
           << std::setw(this->indexWidth)
           << index;

    // restore flags
    output.flags(original);

    output << ".ply";

    return output.str();

  }

};
#endif
