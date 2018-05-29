#include "settings.h"
#include "ApplyModifications.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Ema ema;
    ema.read().from(settings.input);

    ApplyModifications(ema).apply(settings.specifications);

    ema.write().to(settings.output);

    return 0;

}
