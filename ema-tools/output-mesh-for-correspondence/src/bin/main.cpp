#include "model/Model.h"
#include "model/ModelReader.h"
#include "mesh/MeshIO.h"
#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyData.h"
#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/EnergyMinimizer.h"
#include "ema/Ema.h"

#include "settings.h"
#include "Correspondence.h"
#include "CorrespondenceReader.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    // read correspondences information
    Correspondence correspondence = CorrespondenceReader::read(settings.correspondenceFile);
    
    // create energy settings
    fitModel::EnergySettings energySettings;
    
    // use preset neigbors
    energySettings.searchStrategy = fitModel::EnergySettings::SearchStrategy::PRESET;
    for(size_t i = 0; i < correspondence.vertexIndices.size(); ++i) {
    
        // target point i (channel i) corresponds to vertex index vertexIndices[i]
        energySettings.presetTarget.push_back(i);
        energySettings.presetSource.push_back(correspondence.vertexIndices.at(i));
    
    }

    Model model = ModelReader(settings.model).get_model();

    // read EMA data and reduce coil set
    Ema ema;
    ema.read().from(settings.emaFile);
    ema.reduce_coil_set().to(correspondence.emaChannels);

    // transform all coils according to the given command line flags
    if( settings.enforceMidsagittal == true) {

      ema.transform_all_coils().project_to_midsagittal();

    }

    ema.transform_all_coils().scale(settings.scaleFactor);
    ema.transform_all_coils().translate({settings.shiftX, settings.shiftY, settings.shiftZ});

    // generate point cloud from wanted time frame
    Mesh mesh =
      ema.point_cloud().from(correspondence.emaChannels, settings.timeFrame);

    // create energy data
    fitModel::EnergyData energyData(model, mesh);
    
    // create energy
    fitModel::Energy energy(energyData, energySettings);
    
    // create minimizer
    fitModel::EnergyMinimizer minimizer(energy, settings.minimizerSettings);
    
    minimizer.minimize();

    Mesh tongue = model.reconstruct_mesh().for_weights(energyData.speakerWeights, energyData.phonemeWeights);

    MeshIO::write(tongue, settings.outputFile);

    if( settings.outputCoils == true) {

      MeshIO::write(mesh, settings.coilsOutputFile);

    }

    return 0;

}
