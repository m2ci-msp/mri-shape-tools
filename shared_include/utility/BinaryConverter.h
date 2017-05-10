#ifndef __BINARY_CONVERTER_H__
#define __BINARY_CONVERTER_H__

#include <vector>

class BinaryConverter{

public:

  /*--------------------------------------------------------------------------*/

  static unsigned char* convert_to_bytes(
    const std::vector<double>& data, int& size) {

    // compute total size of memory storage and store in the reference
    // variable
    size = data.size() * sizeof(double);

    // allocate storage
    unsigned char* bytes = new unsigned char[size];

    // iterate over values
    for(size_t i = 0; i < data.size(); ++i) {

      // get byte access to the current value
      unsigned char* byte = (unsigned char*) &(data.at(i));

      // store all bytes
      for(size_t j = 0; j < sizeof(double); ++j) {
        bytes[sizeof(double) * i + j] = byte[j];
      } // end for j

    } // end for i

    return bytes;

  }

  /*--------------------------------------------------------------------------*/

  static std::vector<double> convert_to_double(const unsigned char* bytes, int size) {

    int valueAmount = size / sizeof(double);

    std::vector<double> values;

    for(int i = 0; i < valueAmount; ++i) {

      double value;
      unsigned char* byte = (unsigned char*) &value;

      for(size_t j = 0; j < sizeof(double); ++j) {
        byte[j] = bytes[sizeof(double) * i + j];
      }

      values.push_back(value);
    }

    return values;

  }

  /*--------------------------------------------------------------------------*/

};

#endif
