#include <yaml-cpp/yaml.h>
#include "json/json.h"

#include "ErrorDatabaseWriter.h"
#include "training/SampleFileReader.h"
#include "SpeakerGeneralizationMeasurer.h"
#include "PhonemeGeneralizationMeasurer.h"

#include "settings.h"

int main(int argc, char* argv[]){

  Settings settings(argc, argv);

  SampleDataBase trainingDataBase = SampleFileReader::read_from(settings.trainingData);
  SampleDataBase testingDataBase = SampleFileReader::read_from(settings.testingData);

  // evaluate generalization for speaker mode if we have at least two speakers
  if( trainingDataBase.get_speaker_amount() > 1 ) {

    // activate PCA behaviour if only one phoneme component is present
    if( trainingDataBase.get_phoneme_amount() == 1 ) {

      settings.minimizerSettings.noPhonemePCA = false;

    }

    SpeakerGeneralizationMeasurer speakerMeasurer(trainingDataBase, testingDataBase);
    speakerMeasurer.set_energy_settings(settings.energySettings);
    speakerMeasurer.set_minimizer_settings(settings.minimizerSettings);
    speakerMeasurer.set_min_components(settings.minComponents);
    speakerMeasurer.set_restricted_mode(settings.restrictedPhoneme);

    ErrorDatabase speakerErrors = speakerMeasurer.measure();

    ErrorDatabaseWriter writer;

    writer.write(speakerErrors, "components", settings.output + "_speaker.json");

  }

  // evaluate generalization for phoneme mode if we have at least two phonemes
  if( trainingDataBase.get_phoneme_amount() > 1 ) {

    // activate PCA behaviour if only one speaker component is present
    if( trainingDataBase.get_speaker_amount() == 1 ) {

      settings.minimizerSettings.noSpeakerPCA = false;

    }

    PhonemeGeneralizationMeasurer phonemeMeasurer(trainingDataBase, testingDataBase);
    phonemeMeasurer.set_energy_settings(settings.energySettings);
    phonemeMeasurer.set_minimizer_settings(settings.minimizerSettings);
    phonemeMeasurer.set_min_components(settings.minComponents);
    phonemeMeasurer.set_restricted_mode(settings.restrictedSpeaker);

    ErrorDatabase phonemeErrors = phonemeMeasurer.measure();

    ErrorDatabaseWriter writer;

    writer.write(phonemeErrors, "components", settings.output + "_phoneme.json");

  }

  return 0;

}
