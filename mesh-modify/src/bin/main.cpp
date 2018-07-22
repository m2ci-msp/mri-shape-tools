#include "mesh/MeshIO.h"

#include "settings.h"
#include "mesh-modify/ApplyModifications.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Mesh mesh = MeshIO::read(settings.input);

    meshModify::ApplyModifications(mesh).apply(settings.specifications);

    MeshIO::write(mesh, settings.output);

    return 0;

}
