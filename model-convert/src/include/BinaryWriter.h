#ifndef __BINARY_WRITER_H__
#define __BINARY_WRITER_H__

class BinaryWriter{

  public:

  static void write(std::vector<float> values, std::string fileName) {

    std::ofstream output;

    output.open(fileName);

    std::cout << values.size() << std::endl;

    for( auto value: values) {

      union {
        float value;
        char bytes[sizeof(float)];
      } access;


      access.value = value;

      for(int i = 0; i < sizeof(float); ++i) {
        output.put(access.bytes[i]);
      }

    }
  }
};

#endif
