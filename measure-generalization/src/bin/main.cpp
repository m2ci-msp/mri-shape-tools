#include <yaml-cpp/yaml.h>
#include "json/json.h"

#include "ErrorDatabaseWriter.h"
#include "training/SampleFileReader.h"
#include "SpeakerGeneralizationMeasurer.h"
#include "PhonemeGeneralizationMeasurer.h"

#include "settings.h"

int main(int argc, char* argv[]){

  Settings settings(argc, argv);

  SampleDataBase database = SampleFileReader::read_from(settings.input);

  // evaluate generalization for speaker mode if we have at least two speakers
  if( database.get_speaker_amount() > 1 ) {

    SpeakerGeneralizationMeasurer speakerMeasurer(database);
    speakerMeasurer.set_energy_settings(settings.energySettings);
    speakerMeasurer.set_minimizer_settings(settings.minimizerSettings);
    speakerMeasurer.set_min_components(settings.minComponents);
    speakerMeasurer.set_restricted_mode(settings.restrictedPhoneme);

    ErrorDatabase speakerErrors = speakerMeasurer.measure();

    ErrorDatabaseWriter writer;

    writer.write(speakerErrors, "components", settings.output + "_speaker.json");

  }

  // evaluate generalization for phoneme mode if we have at least two phonemes
  if( database.get_phoneme_amount() > 1 ) {

    PhonemeGeneralizationMeasurer phonemeMeasurer(database);
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
