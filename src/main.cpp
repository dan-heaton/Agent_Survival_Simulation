#include <iostream>
#include "../include/Simulation.h"
#include "../include/AdvancedAgent.h"


int main() {

    srand(time(NULL));
    int time_steps = 50;
    int x_bound = 15;
    int y_bound = 15;
    int num_agents = 1;
    int num_obstacles = 0;
    int num_energies = 20;
    bool seek_energy = true;
    bool output_csv = true;
    //In seconds
    int time_delay = 1;
    // If set false, uses BasicAgent; otherwise, uses AdvancedAgent
    bool use_advanced_agents = true;

    Simulation sim(time_steps, x_bound, y_bound, num_agents, num_obstacles, num_energies, seek_energy, output_csv, use_advanced_agents);
    sim.run_simulation(time_delay);
}