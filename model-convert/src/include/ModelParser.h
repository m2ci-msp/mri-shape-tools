#ifndef __MODEL_PARSER_H__
#define __MODEL_PARSER_H__

#include <json/json.h>

class Model{

  public:

    Model(const std::string fileName) {
      this->inputFile.open(fileName);

      lineNumber = 1;

      read_meta();

      read_model();

      read_mode_mean();
    }

    void write_descriptor(std::string fileName) {

      std::ofstream outFile;

      outFile.open(fileName);

      Json::Value descriptor(Json::objectValue);

      // for now, switch roles of identity and expression
      // to make the data compatible with the html framework
      descriptor["totalNumberOfExpressions"] = this->totalNumberOfIdentities;
      descriptor["totalNumberOfIdentities"]  = this->totalNumberOfExpressions;
      descriptor["truncatedNumberOfExpressions"] = this->totalNumberOfIdentities;
      descriptor["truncatedNumberOfIdentities"]  = this->totalNumberOfExpressions;


      Json::StyledStreamWriter writer;
      writer.write(outFile, descriptor);

      outFile.close();

    }


    int numberOfVertices;
    int truncatedNumberOfExpressions;
    int truncatedNumberOfIdentities;
    int totalNumberOfExpressions;
    int totalNumberOfIdentities;

    std::vector<float> model;
    std::vector<float> modeMean;


  private:

    std::stringstream getline() {

      std::string line;
      std::getline(this->inputFile, line);

      ++this->lineNumber;

      std::stringstream stream(line);

      return stream;
    }

    std::stringstream getline(const int& number) {

      while( this->lineNumber < number) {
        getline();
      }

      return getline();

    }

    void read_meta() {

      auto lineStream = getline(2);

      lineStream >> this->numberOfVertices;
      lineStream >> totalNumberOfIdentities;
      lineStream >> totalNumberOfExpressions;

      lineStream = getline(3);

      lineStream >> this->numberOfVertices;
      lineStream >> this->truncatedNumberOfIdentities;
      lineStream >> this->truncatedNumberOfExpressions;

      this->numberOfVertices /= 3;

    }

    void read_model() {

      auto lineStream = getline(7);

      while(lineStream.eof() == false) {
        float value;
        lineStream >> value;
        this->model.push_back(value);
      }

      this->model.pop_back();

    }

    void read_mode_mean() {
      auto lineStream = getline(12);

      while(lineStream.eof() == false) {
        float value;
        lineStream >> value;
        this->modeMean.push_back(value);
      }

      this->modeMean.pop_back();


    }

    int lineNumber;
    std::ifstream inputFile;
};
#endif
