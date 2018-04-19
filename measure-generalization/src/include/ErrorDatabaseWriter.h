#ifndef __ERROR_DATABASE_WRITER_H__
#define __ERROR_DATABASE_WRITER_H__

#include <fstream>

#include <json/json.h>
#include <armadillo>

#include "ErrorDatabase.h"

class ErrorDatabaseWriter{

public:

  void write(
    const ErrorDatabase& dataBase,
    const std::string keyName,
    const std::string fileName
    ) {

    std::ofstream outFile(fileName);

    std::set<int> keys = dataBase.get_keys();

    Json::Value list(Json::arrayValue);

    for(const int& key: keys) {
      Json::Value error(Json::objectValue);

      const arma::vec errors(dataBase.get_errors(key));

      error[keyName] = Json::Value(key);
      error["mean"] = Json::Value(arma::mean(errors));
      error["standardDeviation"] = Json::Value(arma::stddev(errors, 1));

      list.append(error);

    }

    outFile << list << std::endl;

    outFile.close();

  }
};

#endif
