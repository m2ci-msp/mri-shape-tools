#ifndef __TRANSFORMATION_WRITER_H__
#define __TRANSFORMATION_WRITER_H__

#include <fstream>
#include <sstream>
#include <json/json.h>

#include "alignment/RigidTransformation.h"

class TransformationWriter{

public:

  /*-----------------------------------------------------------------------*/

  static void write( const RigidTransformation& transformation, const std::string& fileName ) {

    std::ofstream outFile(fileName);

    // create JSON object
    Json::Value root = Json::objectValue;

    prepare_translation(root, transformation);
    prepare_origin(root, transformation);
    prepare_rotation(root, transformation);
    prepare_scaling(root, transformation);

    Json::StyledStreamWriter writer;
    writer.write(outFile, root);

  }

  /*-----------------------------------------------------------------------*/


  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  static void prepare_translation(Json::Value& output, const RigidTransformation& transformation) {

    std::stringstream stream;

    const arma::vec& translation = transformation.get_translation();

    translation.save(stream, arma::arma_ascii);

    output["translation"] = stream.str();

  }

  /*-----------------------------------------------------------------------*/

  static void prepare_origin(Json::Value& output, const RigidTransformation& transformation) {

    std::stringstream stream;

    const arma::vec& origin = transformation.get_origin();

    origin.save(stream, arma::arma_ascii);

    output["origin"] = stream.str();

  }

  /*-----------------------------------------------------------------------*/

  static void prepare_rotation(Json::Value& output, const RigidTransformation& transformation) {

    std::stringstream stream;

    const arma::mat& rotation = transformation.get_rotation();

    rotation.save(stream, arma::arma_ascii);

    output["rotation"] = stream.str();

  }

  /*-----------------------------------------------------------------------*/

  static void prepare_scaling(Json::Value& output, const RigidTransformation& transformation) {

    std::stringstream stream;

    const double& scaling = transformation.get_scaling();

    output["scaling"] = scaling;

  }

  /*-----------------------------------------------------------------------*/

  TransformationWriter();

  /*-----------------------------------------------------------------------*/

};

#endif
