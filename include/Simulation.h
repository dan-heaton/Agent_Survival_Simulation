#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>
#include "../include/BasicEnvironment.h"

using namespace std;


class Simulation {
    private:
        int time_steps;
        int x_bound;
        int y_bound;
        int num_agents;
        int num_predators;
        int num_obstacles;
        int num_energies;
        bool seek_energy;
        bool output_csv;
        bool use_advanced_agents;
        string file_suffix;
    
    public:
        Simulation();
        void output_csv_row(vector <string> outputs);
        void initialise_csv(BasicEnvironment environment, vector <BasicAgent*> agent_ptrs);
        void run_simulation(int time_delay=0);
};


#endif