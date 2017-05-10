#ifndef __ERROR_DATABASE_H__
#define __ERROR_DATABASE_H__

class ErrorDatabase{

public:

  /*-----------------------------------------------------------------------*/

  void add_error(
    const int& key,
    const double& mean, const double& standardDeviation) {

    this->keys.insert(key);

    this->means[key] = mean;
    this->standardDeviations[key] = standardDeviation;

  }

  /*-----------------------------------------------------------------------*/

  std::set<int> get_keys() const {
    return this->keys;
  }

  /*-----------------------------------------------------------------------*/

  double get_mean(const int& key) const {
    return this->means.at(key);
  }

  /*-----------------------------------------------------------------------*/

  double get_standard_deviation(const int& key) const {
    return this->standardDeviations.at(key);
  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  std::set<int> keys;

  std::map<int, double> means;
  std::map<int, double> standardDeviations;

  /*-----------------------------------------------------------------------*/

};

#endif
