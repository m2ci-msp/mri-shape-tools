#ifndef __FLAG_FIXED_LIST_H__
#define __FLAG_FIXED_LIST_H__


#include <string>
#include <stdexcept>

#include "TypeName.h"
#include "FlagList.h"

/* Template class for a flag that accepts a list of fixed length of
 * arguments
 */
template <typename T>
class FlagFixedList : public FlagList< T >{

  public:

    /*-----------------------------------------------------------------------*/

    FlagFixedList(std::string id, std::vector<T>& argumentList,
      size_t amount, bool optional = false) :
      FlagList< T >(id, argumentList, optional), argumentAmount(amount) {

      // clear the argument list if flag is not optional
      if( optional == false ) {
        this->argumentList.clear();
      }

    }

    /*-----------------------------------------------------------------------*/

    virtual void add_argument(const std::string& token ) {

      // overwrite standard values for optional flag
      if( this->flagOptional == true &&
          this->argumentsRead == false &&
          this->argumentList.size() == this->argumentAmount) {

        this->argumentList.clear();
      }

      // throw exception if arguments were already read
      if( this->argumentsRead == true ) {
        throw std::runtime_error(
          "Arguments for flag --" + this->flagId + " already read."
          );
      }
      
      T parsedValue = FlagList< T >::read_from(token);

      this->argumentList.push_back(parsedValue);

      // set argumentsRead to true if all arguments were read
      if( this->argumentList.size() == this->argumentAmount) {
        this->argumentsRead = true;
      }


    }

    /*-----------------------------------------------------------------------*/

    virtual std::string get_usage_string() const {

      std::string usage;
      usage = "--" + this->flagId;

      for( size_t i = 0; i < this->argumentAmount; ++i) {
        usage += " " + TypeName< T >::to_string();
      }

      if( this->flagOptional == true ) {
        usage = "[ " + usage + " ]";
      }

      return usage;
    }

    /*-----------------------------------------------------------------------*/

  protected:
    size_t argumentAmount;
};
#endif
