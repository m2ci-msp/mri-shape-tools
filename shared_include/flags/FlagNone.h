#ifndef __FLAG_NONE_H__
#define __FLAG_NONE_H__


#include <string>
#include <stdexcept>

#include "TypeName.h"
#include "Flag.h"

/* Template class for a flag that accepts no arguments and serves the purpose
 * of a toggle
 */
class FlagNone : public Flag{

  public:

    /*-----------------------------------------------------------------------*/

    FlagNone(std::string id, bool& value) :
      Flag(id, true), value(value) {

    }

    /*-----------------------------------------------------------------------*/

    virtual void add_argument(const std::string& ) {

      throw std::runtime_error(
          "Flag --" + this->flagId +
        " does not accept any arguments."
        );

    }

    /*-----------------------------------------------------------------------*/
    
    virtual void mark_as_present() {
      this->flagPresent = true;
      this->value = true;
    }

    /*-----------------------------------------------------------------------*/

    bool is_set() const {
      return this->flagPresent;
    }

    /*-----------------------------------------------------------------------*/

    virtual bool is_valid() const {
      // flag is valid if either the flag is present or it is optional
      return (
      ( this->flagPresent ) ||
      ( this->flagOptional )
      );
    }

    /*-----------------------------------------------------------------------*/
    
    virtual std::string get_usage_string() const {

      std::string usage;
      usage = "--" + this->flagId;

      // mark optional flags with "[ ... ]"
      if( this->flagOptional == true ) {
        usage = "[ " + usage + " ]";
      }

      return usage;

    }

    /*-----------------------------------------------------------------------*/

    bool& value;

};
#endif
