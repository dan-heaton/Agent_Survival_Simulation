#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

using namespace std;


void run_simulation(int time_steps, int x_bound, int y_bound, int num_agents, int num_obstacles, 
                    int num_energies, bool seek_energy, bool output_csv, int time_delay=0);


void output_csv_row(vector <string> outputs);


#endif