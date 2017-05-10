#ifndef __PARSER_H__
#define __PARSER_H__

#include <sstream>
#include <string>
#include <stdexcept>

template <typename T> 
class Parser{
  public:
    static T parse_from( const std::string& token ) {

      std::stringstream stream(token);

      T parsedValue;

      stream >> parsedValue;

      // throw exception if token was not parsed successfully
      if( ! stream.eof() || stream.fail() ) {
        throw std::runtime_error("Could not parse value");  
      }

      return parsedValue;

    }

    private:
      Parser() {
      }

};
#endif
