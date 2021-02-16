#include <iostream>
#include <fstream>
#include "../include/Simulation.h"
#include "../include/AdvancedAgent.h"


int main() {

    srand(time(NULL));

    Simulation sim;
    sim.run_simulation();
}