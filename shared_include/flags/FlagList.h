#ifndef __FLAG_LIST_H__
#define __FLAG_LIST_H__


#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Parser.h"
#include "TypeName.h"
#include "Flag.h"

/* Template class for a flag that accepts a list of arbitrary length
 * of arguments
 */
template <typename T>
class FlagList : public Flag{

  public:

    /*-----------------------------------------------------------------------*/

    FlagList(std::string id, std::vector<T>& argumentList,  bool optional = false) :
      Flag(id, optional), argumentList(argumentList) {
    }

    /*-----------------------------------------------------------------------*/

    virtual void add_argument(const std::string& token ) {

      T parsedValue = read_from(token);

      // set argumentsRead to true if at least one argument was read
      this->argumentsRead = true;

      this->argumentList.push_back(parsedValue);

    }

    /*-----------------------------------------------------------------------*/

    std::vector< T > get_arguments() const {
      return this->argumentList;
    }

    /*-----------------------------------------------------------------------*/

    bool is_set() const {
      return flagPresent && argumentsRead;
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
    
    virtual std::string get_usage_string() const {

      std::string usage;
      usage = "--" + this->flagId + " " + TypeName< T >::to_string() + " list";

      // mark optional flags with "[ ... ]"
      if( this->flagOptional == true ) {
        usage = "[ " + usage + " ]";
      }

      return usage;

    }

    /*-----------------------------------------------------------------------*/

  protected:

    /*-----------------------------------------------------------------------*/

    T read_from( const std::string& token ) const {

      try{
        return Parser<T>::parse_from(token);
      }
      catch(std::exception&) {
        throw std::runtime_error("Could not read argument " + 
            std::to_string(this->argumentList.size() + 1) +  
            " for flag --" + this->flagId + ".");
      }
    }

    /*-----------------------------------------------------------------------*/

    std::vector< T >& argumentList;
};
#endif
