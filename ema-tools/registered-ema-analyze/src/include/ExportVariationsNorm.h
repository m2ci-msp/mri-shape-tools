#ifndef __EXPORT_VARIATIONS_NORM_H__
#define __EXPORT_VARIATIONS_NORM_H__

#include <cfloat>
#include <fstream>

#include <json/json.h>
#include "registered-ema/RegisteredEma.h"

class ExportVariationsNorm{

private:

  RegisteredEma& ema;

  std::vector<double> speakerNorm;
  std::vector<double> phonemeNorm;

  double speakerMin;
  double phonemeMin;

  double speakerMax;
  double phonemeMax;

  double speakerMean;
  double phonemeMean;

public:

  ExportVariationsNorm(RegisteredEma& ema) :

    ema(ema) {

    assemble_values();

  }

  void to(const std::string& fileName) const {

    std::ofstream outFile(fileName);

    Json::Value root(Json::objectValue);
    Json::Value speakerList(Json::arrayValue);
    Json::Value phonemeList(Json::arrayValue);

    for(int i = 0; i < this->ema.info().sample_amount(); ++i) {

      speakerList.append(this->speakerNorm.at(i));
      phonemeList.append(this->phonemeNorm.at(i));

    }

    root["speakerVariationsNorms"] = speakerList;
    root["phonemeVariationsNorms"] = phonemeList;

    root["speakerVariationsNormMin"] = this->speakerMin;
    root["speakerVariationsNormMax"] = this->speakerMax;
    root["speakerVariationsNormMean"] = this->speakerMean;

    root["phonemeVariationsNormMin"] = this->phonemeMin;
    root["phonemeVariationsNormMax"] = this->phonemeMax;
    root["phonemeVariationsNormMean"] = this->phonemeMean;

    outFile << root << std::endl;

    outFile.close();

  }

private:

  void assemble_values() {

    this->speakerMin = DBL_MAX;
    this->phonemeMin = DBL_MAX;

    this->speakerMax = -1;
    this->phonemeMax = -1;

    this->speakerMean = 0;
    this->phonemeMean = 0;

    for(int i = 0; i < this->ema.info().sample_amount(); ++i) {

      const double speakerGradientNorm = arma::norm(

                                                    this->ema.access().speaker_variation(i)

                                                    );

      const double phonemeGradientNorm = arma::norm(

                                                    this->ema.access().phoneme_variation(i)

                                                    );

      this->speakerMean += speakerGradientNorm;
      this->phonemeMean += phonemeGradientNorm;

      this->speakerNorm.push_back(speakerGradientNorm);
      this->phonemeNorm.push_back(phonemeGradientNorm);

      this->speakerMin = ( speakerGradientNorm < this->speakerMin )? speakerGradientNorm: this->speakerMin;
      this->phonemeMin = ( phonemeGradientNorm < this->phonemeMin )? phonemeGradientNorm: this->phonemeMin;

      this->speakerMax = ( speakerGradientNorm > this->speakerMax )? speakerGradientNorm: this->speakerMax;
      this->phonemeMax = ( phonemeGradientNorm > this->phonemeMax )? phonemeGradientNorm: this->phonemeMax;

    }

    this->speakerMean /= this->ema.info().sample_amount();
    this->phonemeMean /= this->ema.info().sample_amount();

  }

};

#endif
