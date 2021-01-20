#include <iostream>
#include "BasicAgent.h"
#include "BasicEnvironment.h"

using namespace std;


void run_simulation(int time_steps, int x_bound, int y_bound, int num_agents, int num_obstacles, int num_energies, bool seek_energy) {
    BasicEnvironment environment(x_bound, y_bound, num_obstacles, num_energies);

    // Creates all agents and inserts them into the environment
    vector <BasicAgent*> agent_ptrs;
    for (int i=0; i<num_agents; i++) {
        string agent_name = "Agent " + to_string(i+1);
        // Needs dynamic allocation to ensure objects survive out of the 'for' scope
        BasicAgent* agent_ptr = new BasicAgent(agent_name, x_bound, y_bound);
        environment.insert_agent(agent_ptr);
        agent_ptrs.push_back(agent_ptr);
    }
    environment.visualise();

    // For the required number of time steps, have all agents move randomly across the board
    for (int i=0; i<time_steps; i++) {
        for (BasicAgent* agent_ptr: agent_ptrs) {
            if (seek_energy) {
                agent_ptr->seek_energy();
            }
            else {
                agent_ptr->move_random();
            }
        }
        environment.update();
        environment.visualise();
    }

    //Kills off the agents, now that we're done with the simulation
    for (BasicAgent* agent_ptr: agent_ptrs) {
        delete agent_ptr;
    }

    cout << "Simulation complete!" << endl;
}


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