#include <iostream>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include "../include/BasicAgent.h"
#include "../include/BasicEnvironment.h"
#include "../include/Simulation.h"


using namespace std;


Simulation::Simulation(int time_steps, int x_bound, int y_bound, 
                       int num_agents, int num_obstacles, int num_energies, 
                       bool seek_energy, bool output_csv) 
                       :time_steps(time_steps), x_bound(x_bound), y_bound(y_bound), 
                       num_agents(num_agents), num_obstacles(num_obstacles), num_energies(num_energies), 
                       seek_energy(seek_energy), output_to_csv(output_to_csv) {

                           if (output_to_csv) {
                               //Generates timedate as string in format "YYYY-MM-DD-HH-MM-SS" to use in file name
                                time_t t = std::time(0);
                                tm* now = std::localtime(&t);
                                file_suffix = to_string(now->tm_year + 1900) + '-' + to_string(now->tm_mon + 1) + '-' 
                                              + to_string(now->tm_mday) + '-' + to_string(now->tm_hour) + '-' 
                                              + to_string(now->tm_min) + '-' + to_string(now->tm_sec);
                           }
                       }


void Simulation::output_csv_row(vector <string> outputs) {
    string file_name = "outputs/simulation_" + file_suffix + ".csv";
    ofstream file(file_name, ios::app);

    for (string output: outputs) { 
        file << output << "\t";
    }

    file << "\n";
    file.close();
}


void Simulation::initialise_csv(BasicEnvironment environment, vector <BasicAgent*> agent_ptrs) {

    //Sets up 1st line for initial settings headers
    vector <string> first_outputs = {"Time Steps", "X-bound", "Y-bound", "# Agents", "# Obstacles", "# Energies", "Seek Energy?"};
    for (int i=0; i<num_obstacles; i++) {
        for (char dimension: vector <char> {'X', 'Y'}) {
            first_outputs.push_back("Obstacle " + to_string(i+1) + " " + dimension + "-Pos");
        }
    }
    for (int i=0; i<num_energies; i++) {
        for (char dimension: vector <char> {'X', 'Y'}) {
            first_outputs.push_back("Energy " + to_string(i+1) + " " + dimension + "-Pos");
        }
    }
    output_csv_row(first_outputs);

    //Sets up 2nd line for initial settings
    vector <string> second_outputs = {to_string(time_steps), to_string(x_bound), to_string(y_bound), to_string(num_agents), 
                                        to_string(num_obstacles), to_string(num_energies), seek_energy ? "true": "false"};
    for (Obstacle obst: environment.get_obstacles()) {
        second_outputs.insert(second_outputs.end(), {to_string(obst.x_pos), to_string(obst.y_pos)});
    }
    for (Energy engy: environment.get_energy_sources()) {
        second_outputs.insert(second_outputs.end(), {to_string(engy.x_pos), to_string(engy.y_pos)});
    }                               
    output_csv_row(second_outputs);

    //Sets up 3rd line for other headers
    vector <string> third_outputs = {"Time Step", "# Energies Remaining"};
    for (int i=0; i<num_agents; i++) {
        for (char dimension: vector <char> {'X', 'Y'}) {
            third_outputs.push_back("Agent " + to_string(i+1) + " " + dimension + "-Pos");
        }
    }
    output_csv_row(third_outputs);

    //Inserts the initial simulation setup at time 0
    vector <string> fourth_outputs;
    fourth_outputs.insert(fourth_outputs.end(), {"0", to_string(environment.number_energies_remaining())});
    for (BasicAgent* agent_ptr: agent_ptrs) {
        fourth_outputs.insert(fourth_outputs.end(), {to_string(agent_ptr->get_x_pos()), to_string(agent_ptr->get_y_pos())});
    }
    output_csv_row(fourth_outputs);   
}


void Simulation::run_simulation(int time_delay) {

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


    //Sets up the inital .csv output file w/ 1st line for initial settings headers, 2nd for initial settings, 
    //3rd for other headers, and 4th for time 0 setting
    if (output_to_csv) {
        initialise_csv(environment, agent_ptrs);
    }

    // For the required number of time steps, have all agents move randomly across the board
    for (int i=0; i<time_steps; i++) {
        //Delays output of new board so shows running in 'real-time' if necessary
        sleep(time_delay);

        vector <string> outputs;
        for (BasicAgent* agent_ptr: agent_ptrs) {
            if (seek_energy) {
                agent_ptr->seek_energy();
            }
            else {
                agent_ptr->move_random();
            }
            outputs.insert(outputs.end(), {to_string(agent_ptr->get_x_pos()), to_string(agent_ptr->get_y_pos())});
        }
        environment.update();
        environment.visualise();
        outputs.insert(outputs.begin(), {to_string(i+1), to_string(environment.number_energies_remaining())});

        //Output the simulation state at time 'i+1' to the output .csv
        if (output_to_csv) {
            output_csv_row(outputs);
        }   
    }

    //Kills off the agents, now that we're done with the simulation
    for (BasicAgent* agent_ptr: agent_ptrs) {
        delete agent_ptr;
    }

    cout << "Simulation complete!" << endl;
}