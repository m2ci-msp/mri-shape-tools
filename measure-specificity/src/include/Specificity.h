#ifndef __SPECIFICITY_H__
#define __SPECIFICITY_H__

#include "tensor/TensorBuilder.h"
#include "model/ModelBuilder.h"
#include "training/SampleFileReader.h"
#include "training/TrainingDataBuilder.h"
#include "SpeakerSpecificityMeasurer.h"
#include "PhonemeSpecificityMeasurer.h"
#include "ErrorDatabaseWriter.h"

#include "SubsetCollection.h"

#include "settings.h"


class Specificity{

public:

  /*-----------------------------------------------------------------------*/

  Specificity(const Settings& settings) : settings(settings) {

    this->database = SampleFileReader::read_from(settings.input);

    TrainingDataBuilder trainingBuilder(this->database);
    TrainingData trainingData = trainingBuilder.build();

    Tensor tensor = TensorBuilder::build_tensor(trainingData);

    this->builder.set_faces(database.get_faces());
    this->builder.set_tensor(tensor);
    this->builder.set_origin(trainingData.mean);

    this->model = builder.build();

    prepare_subset_collection();

    prepare_mesh_collection();

  }

  /*-----------------------------------------------------------------------*/

  void measure() {

    if( this->settings.deactivateSpeaker == false ) {
      measure_speaker_specificity();
    }

    if( this->settings.deactivatePhoneme == false ) {
      measure_phoneme_specificity();
    }

    if( this->settings.evaluateFixedPhoneme == true ) {
      measure_speaker_specificity_with_fixed_phoneme();
    }

  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  void prepare_subset_collection() {

    if(this->settings.subsetsPresent == true){

      // add subsets to collection
      for(const std::string& fileName: this->settings.subsets) {
        this->subsetCollection.add_subset(fileName);
      }
    } else {

      // no subsets provided -> create set with all vertices

      std::vector<int> indices;
      std::string name = "all";
      const Mesh mesh = this->database.get_all_meshes().at(0);

      for(unsigned int i = 0; i < mesh.get_vertices().size(); ++i) {
        indices.push_back(i);
      }

      this->subsetCollection.add_subset(name, indices);

    }

    return;

  }

  /*-----------------------------------------------------------------------*/

  void prepare_mesh_collection() {

    // by default, only use all meshes of the training data
    this->meshCollection = this->database.get_all_meshes();

    // check if we want to add additional meshes
    if( this->settings.useAdditionalData == true) {

      SampleDataBase additionalData = SampleFileReader::read_from(settings.additionalData);

      // add meshes
      for(const Mesh& mesh: additionalData.get_all_meshes() ) {

        this->meshCollection.push_back(mesh);

      }

    }

  }

  /*-----------------------------------------------------------------------*/

  void measure_speaker_specificity() {

    // evaluate specificity for speaker
    SpeakerSpecificityMeasurer speakerMeasurer(this->meshCollection, this->builder);
    speakerMeasurer.set_sample_amount(this->settings.sampleAmount);
    speakerMeasurer.set_max_components(this->database.get_speaker_ids().size());
    speakerMeasurer.set_truncated_pose_mode(this->settings.truncatedPoseMode);

    std::vector<Subset> subsets = this->subsetCollection.get_subsets();
    speakerMeasurer.measure(subsets);

    output_measurements(subsets, "speaker");

    return;

  }

  /*-----------------------------------------------------------------------*/

  void measure_phoneme_specificity() {

    // evaluate specificity for phoneme
    PoseSpecificityMeasurer phonemeMeasurer(this->meshCollection, this->builder);
    phonemeMeasurer.set_sample_amount(this->settings.sampleAmount);
    phonemeMeasurer.set_max_components(this->database.get_phoneme_ids().size());
    phonemeMeasurer.set_truncated_speaker_mode(this->settings.truncatedSpeakerMode);

    std::vector<Subset> subsets = this->subsetCollection.get_subsets();
    phonemeMeasurer.measure(subsets);

    output_measurements(subsets, "phoneme");

    return;

  }

  /*-----------------------------------------------------------------------*/

  // ATTENTION: this measurement does not use any additional meshes
  void measure_speaker_specificity_with_fixed_phoneme() {

    const arma::mat originalWeights = this->model.data().get_original_phoneme_weights();
    unsigned int i = 0;
    for(const std::string& phonemeId: this->database.get_phoneme_ids()) {

      const arma::vec phonemeWeights = originalWeights.row(i).t();
      const std::vector<Mesh>& meshes = this->database.get_phoneme_meshes(phonemeId);

      SpeakerSpecificityMeasurer speakerMeasurer(meshes, this->builder);
      speakerMeasurer.set_sample_amount(this->settings.sampleAmount);
      speakerMeasurer.set_max_components(this->database.get_speaker_ids().size());
      speakerMeasurer.set_fixed_pose(phonemeWeights);

      std::vector<Subset> subsets = this->subsetCollection.get_subsets();
      speakerMeasurer.measure(subsets);

      output_measurements(subsets, phonemeId + "_speaker");

      // increment index for weight access
      ++i;

    }

    return;

  }

  /*-----------------------------------------------------------------------*/

  void output_measurements(const std::vector<Subset>& subsets,
                           const std::string& id) {

    ErrorDatabaseWriter writer;

    for(const Subset& subset: subsets) {
      writer.write(subset.errorDataBase, "components",
                   this->settings.output + "_" + subset.name + "_" + id + ".json");
    }

    return;

  }

  /*-----------------------------------------------------------------------*/

  SampleDataBase database;
  ModelBuilder builder;
  const Settings& settings;

  Model model;
  SubsetCollection subsetCollection;
  std::vector<Mesh> meshCollection;

  /*-----------------------------------------------------------------------*/

};

#endif
