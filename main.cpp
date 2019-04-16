#include "Runner.h"
#include "RunnerConfig.h"

#include <iostream>
#include <cstdlib>

using namespace std;


// This example reads the configuration file 'example.cfg' and displays
// some of its contents.

int main(int argc, char **argv) {
    RunnerConfig cfg;
    if (!cfg.load()){
        return EXIT_FAILURE;
    }
    Runner(&cfg).start();
}

