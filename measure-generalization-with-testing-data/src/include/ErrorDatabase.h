#ifndef __ERROR_DATABASE_H__
#define __ERROR_DATABASE_H__

class ErrorDatabase{

public:

  /*-----------------------------------------------------------------------*/

  void add_error(const int& key, const double& error) {

    this->keys.insert(key);

    this->dataBase[key].push_back(error);

  }

  /*-----------------------------------------------------------------------*/

  std::set<int> get_keys() const {
    return this->keys;
  }

  /*-----------------------------------------------------------------------*/

  std::vector<double> get_errors(const int& key) const {
    return this->dataBase.at(key);
  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  std::set<int> keys;
  std::map< int, std::vector<double> > dataBase;

  /*-----------------------------------------------------------------------*/

};

#endif
