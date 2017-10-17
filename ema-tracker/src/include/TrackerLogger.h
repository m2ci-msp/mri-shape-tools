#ifndef __TRACKER_LOGGER_H__
#define __TRACKER_LOGGER_H__

#include <vector>
#include <json/json.h>

#include "TrackerData.h"
#include "TrackerState.h"

class TrackerLogger{

public:

  /*--------------------------------------------------------------------------*/

  TrackerLogger(
    const TrackerData& trackerData,
    const TrackerState& trackerState
    ) : trackerData(trackerData), trackerState(trackerState) {

    this->loggedData = Json::Value(Json::arrayValue);

  }

  /*--------------------------------------------------------------------------*/

  Json::Value get_logged_data() const {
    return this->loggedData;
  }

  /*--------------------------------------------------------------------------*/

  void log() {

    Json::Value logData(Json::objectValue);

    log_time(logData);
    log_energy(logData);
    log_source(logData);
    log_target(logData);
    log_error(logData);
    log_weights(logData);

    this->loggedData.append(logData);

  }

  /*--------------------------------------------------------------------------*/


private:

  /*--------------------------------------------------------------------------*/

  void log_error(Json::Value& logData) const {

    const arma::vec& speakerWeights = this->trackerData.speakerWeight;
    const arma::vec& phonemeWeights = this->trackerData.phonemeWeight;

    const Mesh mesh =
      this->trackerData.currentModel.reconstruct_mesh().for_weights(
        speakerWeights,
        phonemeWeights
        );

    double result = 0;

    const std::vector<arma::vec>& source = mesh.get_vertices();
    const std::vector<arma::vec>& target = this->trackerData.target.get_vertices();

    for(unsigned int i = 0; i < source.size(); ++i) {
      const arma::vec difference = source.at(i) - target.at(i);
      result += arma::norm(difference);
    }

    result /= source.size();

    logData["error"] = result;

  }

  /*--------------------------------------------------------------------------*/

  void log_time(Json::Value& logData) const {
    logData["timeStamp"] = this->trackerData.currentTime;
  }

  /*--------------------------------------------------------------------------*/

  void log_energy(Json::Value& logData) const {
    logData["energy"] = this->trackerData.energy;
  }

  /*--------------------------------------------------------------------------*/

  void log_target(Json::Value& logData) const {

    Json::Value target(Json::arrayValue);

    for(const arma::vec vertex: this->trackerData.target.get_vertices()){

      Json::Value coordinates(Json::objectValue);

      coordinates["x"] = vertex(0);
      coordinates["y"] = vertex(1);
      coordinates["z"] = vertex(2);

      target.append(coordinates);

    }

    logData["target"] = target;

  }

  /*--------------------------------------------------------------------------*/

  void log_source(Json::Value& logData) const {

    Json::Value source(Json::arrayValue);

    const arma::vec& speakerWeights = this->trackerData.speakerWeight;
    const arma::vec& phonemeWeights = this->trackerData.phonemeWeight;

    const Mesh mesh =
      this->trackerData.currentModel.reconstruct_mesh().for_weights(
        speakerWeights,
        phonemeWeights
        );

    for(const arma::vec vertex: mesh.get_vertices()){

      Json::Value coordinates(Json::objectValue);

      coordinates["x"] = vertex(0);
      coordinates["y"] = vertex(1);
      coordinates["z"] = vertex(2);

      source.append(coordinates);

    }

    logData["source"] = source;

  }


  /*--------------------------------------------------------------------------*/

  void log_weights(Json::Value& logData) const {

    // select right speaker weights
    arma::vec speakerWeights =
      (this->trackerState.isPCA) ? this->trackerData.fixedSpeakerWeights : this->trackerData.speakerWeight;

    arma::vec phonemeWeights = this->trackerData.phonemeWeight;

    arma::vec speakerVariations = speakerWeights;
    arma::vec phonemeVariations = phonemeWeights;

    this->trackerData.originalModel.convert().to_variations(
      speakerVariations,
      phonemeVariations
      );

    Json::Value jsonSpeakerWeights(Json::arrayValue);
    Json::Value jsonSpeakerVariations(Json::arrayValue);

    Json::Value jsonPhonemeWeights(Json::arrayValue);
    Json::Value jsonPhonemeVariations(Json::arrayValue);

    for(unsigned int i = 0; i < speakerWeights.n_elem; ++i) {

      jsonSpeakerWeights.append(speakerWeights(i));
      jsonSpeakerVariations.append(speakerVariations(i));

    }

    for(unsigned int i = 0; i < phonemeWeights.n_elem; ++i) {

      jsonPhonemeWeights.append(phonemeWeights(i));
      jsonPhonemeVariations.append(phonemeVariations(i));

    }

    logData["speakerWeights"] = jsonSpeakerWeights;
    logData["speakerVariations"] = jsonSpeakerVariations;

    logData["phonemeWeights"] = jsonPhonemeWeights;
    logData["phonemeVariations"] = jsonPhonemeVariations;

  }

  /*--------------------------------------------------------------------------*/

  const TrackerData& trackerData;
  const TrackerState& trackerState;

  Json::Value loggedData;

  /*--------------------------------------------------------------------------*/

};

#endif
