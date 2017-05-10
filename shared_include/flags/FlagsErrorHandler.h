#ifndef __FLAGS_ERROR_HANDLER_H__
#define __FLAGS_ERROR_HANDLER_H__

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Flag.h"

/* class for handling errors during flag parsing */
class FlagsErrorHandler {

  public:

    FlagsErrorHandler(
        const std::string& programName,
        const std::vector<Flag*>& requiredFlags,
        const std::vector<Flag*>& optionalFlags
        ) :
      programName(programName),
      requiredFlags(requiredFlags),
      optionalFlags(optionalFlags) {

        this->baseIndent = std::string("    ");
        this->indentSize = this->baseIndent.length() + this->programName.length();

      }

    /*------------------------------------------------------------------------*/

    void handle(std::exception& ex) {

      print_error(ex.what());
      print_usage();
      exit(1);

    }

    /*------------------------------------------------------------------------*/

  protected:

    void print_error( const std::string& error) const {

      std::cerr << std::endl;
      std::cerr << "Problem parsing flags:" << std::endl;
      std::cerr << std::endl;
      std::cerr << this->baseIndent << error << std::endl;
      std::cerr << std::endl;

    }

    /*------------------------------------------------------------------------*/

    void print_flags_usage(const std::vector<Flag*>& flags) {

      for(const auto& flag: flags) {

        const std::string usageString = flag->get_usage_string();
        std::cerr << " " << usageString;

        this->lineLength += 1 + usageString.length();

        if( this->lineLength > 80 ) {

          std::cerr << std::endl;

          for( size_t i = 0; i < this->indentSize; ++i) {
            std::cerr << " ";
          }

          this->lineLength = indentSize;
        }

      }



    }

    /*------------------------------------------------------------------------*/

    void print_usage() {

      std::cerr << "Usage:" << std::endl;
      std::cerr << std::endl;
      std::cerr << this->baseIndent << this->programName;

      this->lineLength = this->indentSize;

      print_flags_usage(this->requiredFlags);
      print_flags_usage(this->optionalFlags);

      std::cerr << std::endl;
      std::cerr << std::endl;

    }

    /*------------------------------------------------------------------------*/

    const std::string programName;
    const std::vector<Flag*> requiredFlags;
    const std::vector<Flag*> optionalFlags;

    size_t lineLength;
    std::string baseIndent;
    size_t indentSize;

};
#endif
