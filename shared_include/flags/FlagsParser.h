#ifndef __FLAGS_PARSER_H__
#define __FLAGS_PARSER_H__

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <stdexcept>

#include "Flag.h"
#include "FlagsErrorHandler.h"

/* class for parsing flags from the command line */
class FlagsParser{
  public:

    /*-----------------------------------------------------------------------------*/

    FlagsParser(const std::string programName) :
      programName(programName) {
      }

    /*-----------------------------------------------------------------------------*/

    /* define a new flag that should be considered by the parser */
    void define_flag(Flag* val) {

      // throw exception if flag with that id was already defined
      if( flags.find(val->get_id()) != this->flags.cend() ) {
        throw std::logic_error(
            "Flag with id " + val->get_id() + " is already defined."
          );
      }

      // save flag as either an optional flag or a required one
      if( val->is_optional() == true ) {
        this->optionalFlags.push_back(val);
      }
      else {
        this->requiredFlags.push_back(val);
      }

      // also record flag in lookup table
      this->flags[val->get_id()] = val;
    }

    /*-----------------------------------------------------------------------------*/

    void parse_from_command_line(int argc, char* argv[]) {

      try {
        // turn command line arguments into strings
        for(int i = 1; i < argc; ++i) {
          this->tokens.push_back(std::string(argv[i]));
        }

        process_tokens();
      }
      catch ( std::exception& ex ) {

        FlagsErrorHandler handler(
            this->programName, this->requiredFlags, this->optionalFlags
          );
        handler.handle(ex);
      }
    }

    /*-----------------------------------------------------------------------------*/

  private:
    
    /*-----------------------------------------------------------------------------*/

    FlagsParser() {
    }

    /*-----------------------------------------------------------------------------*/

    bool found_new_flag() {

      std::regex flagFormat("--(.*)");
      std::smatch match;

      const std::string currentToken = this->tokens.at(this->currentTokenIndex);

      // check if we found a new flag
      if( std::regex_match(currentToken, match, flagFormat) ) {

        this->currentFlagId = match[1];

        // throw exception if flag has no id
        if( currentFlagId.empty() ) {
          throw std::runtime_error("Flag with no id is not supported.");
        }

        // throw exception if flag is not known
        if( flags.find(this->currentFlagId) == this->flags.cend() ) {
          throw std::runtime_error("Unknown flag: " + this->currentFlagId);
        }

        return true;
      }

      return false;
    }

    /*-----------------------------------------------------------------------------*/

    void add_new_argument_to_flag() {

      Flag* currentFlag;
      const std::string currentToken = this->tokens.at(currentTokenIndex);

      try {
        currentFlag = this->flags.at(this->currentFlagId);
      }
      catch (const std::exception& ) {
        throw std::runtime_error(
            "Encountered argument \"" + currentToken +
            "\" not belonging to any flag."
            );
      }


      currentFlag->add_argument(currentToken);

    }

    /*-----------------------------------------------------------------------------*/

    void process_tokens() {

      // process all tokens
      for(
          currentTokenIndex = 0;
          currentTokenIndex < this->tokens.size(); ++currentTokenIndex
         ) {

        if( found_new_flag() == false ) {

          add_new_argument_to_flag();

        }
        else {

          Flag* currentFlag = this->flags.at(this->currentFlagId);
          currentFlag->mark_as_present();

        }

      }

      // check if all flags are valid
      for( auto elem: this->flags) {

        const Flag* flag = elem.second;

        // throw exception if there is something wrong with the flag
        if(flag->is_valid() == false) {
          throw std::runtime_error(
              "Flag --" + elem.second->get_id() + " is not valid."
              );
        }
      }
    }

    /*-----------------------------------------------------------------------------*/

    std::string programName;

    size_t currentTokenIndex;
    std::string currentFlagId;

    // lookup table: flagId -> flag
    std::map<std::string, Flag*> flags;

    std::vector<Flag*> optionalFlags;
    std::vector<Flag*> requiredFlags;

    std::vector<std::string> tokens;


};
#endif
