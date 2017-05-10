#ifndef __FLAG_SINGLE_H__
#define __FLAG_SINGLE_H__

#include <string>
#include <sstream>

#include "Parser.h"
#include "TypeName.h"
#include "Flag.h"

/* Template class for flag that accepts only a single argument */
template <typename T>
class FlagSingle : public Flag {

  public:

    /*-----------------------------------------------------------------------*/

    FlagSingle(std::string id, T& value, bool optional = false) :
      Flag(id, optional), value(value) {
    }

    /*-----------------------------------------------------------------------*/

    T get_value() const {
      return this->value;
    }

    /*-----------------------------------------------------------------------*/

    virtual void add_argument(const std::string& token ) {

      // throw exception if arguments were already read
      if( this->argumentsRead == true ) {
        throw std::runtime_error(
          "Arguments for flag --" + this->flagId + " already read."
          );
      }

      try{
        T parsedValue = Parser<T>::parse_from(token);

        // set argumentsRead to true
        this->argumentsRead = true;

        this->value = parsedValue;
      }
      catch(std::exception&) {
        throw std::runtime_error(std::string("Could not read argument ") + 
            "for flag --" + this->flagId + ".");
      }

    }

    /*-----------------------------------------------------------------------*/

    virtual std::string get_usage_string() const {

      std::string usage;
      usage = "--" + this->flagId + " " + TypeName< T >::to_string();

      // mark optional flags with "[ ... ]" and show standard value
      if( this->flagOptional == true ) {
        // use string stream to get string version of standard value 
        // and generate usage string
        std::stringstream stream;
        stream << this->value;
        usage = "[ " + usage + " = " + stream.str() + " ]";
      }

      return usage;

    }

    /*-----------------------------------------------------------------------*/

    virtual bool is_valid() const {
      // flag is valid if either the flag is present and its arguments were
      // read or it is optional and not present 
      return (
      ( this->argumentsRead && this->flagPresent ) ||
      ( !this->flagPresent  && this->flagOptional )
      );
    }

    /*-----------------------------------------------------------------------*/

    private:
      T& value;

};
#endif
