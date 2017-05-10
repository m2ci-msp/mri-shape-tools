#ifndef __FLAG_H__
#define __FLAG_H__

#include <string>

/* Abstract base class for a flag */
class Flag{
  public:

    /*-----------------------------------------------------------------------*/

    Flag( std::string id, bool optional = false ) :
      flagId(id), flagOptional(optional) {

        this->argumentsRead = false;
        this->flagPresent = false;

      }

    /*-----------------------------------------------------------------------*/

    virtual std::string get_id() const {
      return this->flagId;
    }

    /*-----------------------------------------------------------------------*/

    virtual void mark_as_present() {
      this->flagPresent = true;
    }

    /*-----------------------------------------------------------------------*/

    bool is_optional() const {
      return this->flagOptional;
    }

    /*-----------------------------------------------------------------------*/

    bool is_present() const {
      return this->flagPresent;
    }

    /*-----------------------------------------------------------------------*/

    virtual void add_argument(const std::string& value ) = 0;
    virtual bool is_valid() const = 0;
    virtual std::string get_usage_string() const = 0;

    /*-----------------------------------------------------------------------*/

  protected:

    /*-----------------------------------------------------------------------*/

    Flag(){
      this->flagOptional = false;
      this->argumentsRead = false;
      this->flagPresent = false;
    }

    /*-----------------------------------------------------------------------*/

    std::string flagId;
    bool flagOptional;
    bool argumentsRead;
    bool flagPresent;
};
#endif
