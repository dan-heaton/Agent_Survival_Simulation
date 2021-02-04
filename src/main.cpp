#include <iostream>
#include "../include/Simulation.h"
#include "../include/AdvancedAgent.h"


int main() {

    srand(time(NULL));

    //In seconds
    int time_delay = 1;

    Simulation sim;
    sim.run_simulation(time_delay);
}