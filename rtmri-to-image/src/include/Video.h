#ifndef __PGM_LIST_READER__
#define __PGM_LIST_READER__

#include <fstream>
#include <vector>
#include <string>

#include "image/Image.h"

class PGMListReader{

private:

  std::vector<std::string> fileList;

  int width;
  int height;

  int maximum;

  std::vector<double> values;

public:

  PGMListReader(const std::vector<std::string>& fileList) :
    fileList(fileList) {

  }

  Image read_list() {

    process_files();

    Image original;

    original.create()\
      .with_dimension(this->width, this->height, this->fileList.size())\
      .with_boundary(0, 0, 0)\
      .image_from(this->values);

    Image result(original);

    for(int x = 0; x < this->width; ++x ) {

      for( int y = 0; y < this->height; ++y) {

        for( int z = 0; z < this->fileList.size(); ++z) {

          result.access().at_grid(x, y, z) = original.access().at_grid(y, x, z);

        }

      }


    }

    return result;

  }

private:

  void process_files() {

    this->values.clear();

    std::string firstFile = this->fileList.front();
    std::ifstream inFile(firstFile);

    read_header(inFile);

    inFile.close();

    for(const std::string& fileName: this->fileList) {

      std::cout << fileName << std::endl;
      read_file(fileName);

    }

  }

  void read_file(const std::string& fileName) {

    std::ifstream inFile(fileName);

    verify_header(inFile);

    unsigned char value;

    value = inFile.get();

    while( (inFile.eof() == false ) && ( inFile.fail() == false ) ) {

      this->values.push_back(value);

      value = inFile.get();


    }

    inFile.close();

  }

  void verify_header(std::ifstream& inFile) {

    std::string line;

    std::getline(inFile, line);

    if(line != "P5") {

      throw std::runtime_error("Unsupported file.");

    }

    int width, height, maximum;
    inFile >> width;
    inFile >> height;

    inFile >> maximum;

    if( width != this->width || height != this->height) {

      throw std::runtime_error("Incompatible files.");

    }

    inFile.get();

  }


  void read_header(std::ifstream& inFile) {

    std::string line;

    std::getline(inFile, line);

    if(line != "P5") {

      throw std::runtime_error("Unsupported file.");

    }

    inFile >> this->width;
    inFile >> this->height;

    inFile >> this->maximum;

  }

};
#endif
