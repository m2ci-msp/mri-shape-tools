#ifndef __EXPORT_FREQUENCY_H__
#define __EXPORT_FREQUENCY_H__

#include <fstream>

#include <armadillo>
#include <json/json.h>

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"
#include "ema/EmaInfo.h"

class ExportFrequency{

public:

  ExportFrequency(const EmaCoilAccess& coil, const EmaInfo& info) : 
  
    coil(coil),
    info(info) {
  
  }

  void to(const std::string& fileName) const {
  
    std::ofstream outFile(fileName);
  
    Json::Value jsonList(Json::arrayValue);

    arma::vec x = arma::zeros( this->info.sample_amount() );
    arma::vec y = arma::zeros( this->info.sample_amount() );
    arma::vec z = arma::zeros( this->info.sample_amount() );

    for(int i = 0; i < this->info.sample_amount(); ++i) {

      x(i) = this->coil.position(i)(0);
      y(i) = this->coil.position(i)(1);
      z(i) = this->coil.position(i)(2);

    }

    arma::cx_mat frequencyX = arma::fft(x);
    arma::cx_mat frequencyY = arma::fft(y);
    arma::cx_mat frequencyZ = arma::fft(z);
 
    // only output the meaningful frequencies, 0 to sampleAmount / 2
    for(int i = 0; i <= this->info.sample_amount() / 2; ++i) {
  
      Json::Value entry(Json::objectValue);

      entry["frequency"] = (double) i / this->info.sample_amount() * this->info.sampling_frequency();

      entry["x"] = sqrt( pow(frequencyX(i).real(), 2) + pow(frequencyX(i).imag(), 2) );
      entry["y"] = sqrt( pow(frequencyY(i).real(), 2) + pow(frequencyY(i).imag(), 2) );
      entry["z"] = sqrt( pow(frequencyZ(i).real(), 2) + pow(frequencyZ(i).imag(), 2) );
  
      jsonList.append(entry);
  
    }
  
    outFile << jsonList << std::endl;
  
    outFile.close();
  
  }

private:

  const EmaCoilAccess& coil;
  const EmaInfo& info;

};
#endif
