#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>
#include "../include/BasicEnvironment.h"

using namespace std;


class Simulation {
    private:
        int time_delay;
        int time_steps;
        int x_bound;
        int y_bound;
        int min_energy_to_replicate;
        int max_energy_to_replicate;
        int max_num_lookaheads;
        int num_agents;
        int num_predators;
        int num_obstacles;
        int num_energies;
        int agent_choice;
        bool seek_energy;
        bool output_csv;
        string file_suffix;
        //Needed for when an agent replicates to set a new, unique name based on new num of agents
        void increment_num_agents();
    
    public:
        Simulation();
        void output_csv_row(vector <string> outputs);
        void initialise_csv(BasicEnvironment environment, vector <BasicAgent*> agent_ptrs);
        void run_simulation();
};


#endif