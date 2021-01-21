#include <iostream>
#include "Simulation.h"


int main() {

    srand(time(NULL));
    int time_steps = 50;
    int x_bound = 15;
    int y_bound = 15;
    int num_agents = 1;
    int num_obstacles = 0;
    int num_energies = 20;

    run_simulation(time_steps, x_bound, y_bound, num_agents, num_obstacles, num_energies, true);
}