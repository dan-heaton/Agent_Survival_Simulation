#include <iostream>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include "../include/AdvancedAgent.h"
#include "../include/BasicAgent.h"
#include "../include/BasicEnvironment.h"
#include "../include/ReplicationAgent.h"
#include "../include/Simulation.h"


using namespace std;


Simulation::Simulation() {

    ifstream file("config.txt");

    //Makes mapping of simulation attributes to their respective attribute values
    map <string, string> config_pairs;
    string config_line;
    while (getline(file, config_line))
    {
        //All attributes to attribute values are expecting the exact delimiter below (otherwise, will cause error)
        string line_delim = " = ";
        int equals_pos = config_line.find(line_delim);
        string attr = config_line.substr(0, equals_pos);
        string attr_val = config_line.substr(equals_pos + line_delim.size(), config_line.size());
        config_pairs[attr] = attr_val;
    }

    //Initialises all model attributes from the configuration mapping 
    time_delay = stoi(config_pairs["time_delay"]);
    time_steps = stoi(config_pairs["time_steps"]);
    x_bound = stoi(config_pairs["x_bound"]);
    y_bound = stoi(config_pairs["y_bound"]);
    min_energy_to_replicate = stoi(config_pairs["min_energy_to_replicate"]);
    max_energy_to_replicate = stoi(config_pairs["max_energy_to_replicate"]);
    max_num_lookaheads = stoi(config_pairs["max_num_lookaheads"]);
    num_agents = stoi(config_pairs["num_agents"]);
    num_predators = stoi(config_pairs["num_predators"]);
    num_obstacles = stoi(config_pairs["num_obstacles"]);
    num_energies = stoi(config_pairs["num_energies"]);
    // Set to 1 for BasicAgent, 2 for AdvancedAgent, or 3 for ReplicationAgent
    agent_choice = stoi(config_pairs["agent_choice"]);
    seek_energy = (config_pairs["seek_energy"] == "true") ? true : false;
    output_csv = (config_pairs["output_csv"] == "true") ? true : false;
    output_dir = config_pairs["output_dir"];
    
    
    if (output_csv) {
        //Generates timedate as string in format "YYYY-MM-DD-HH-MM-SS" to use in file name
        time_t t = std::time(0);
        tm* now = std::localtime(&t);
        file_suffix = to_string(now->tm_year + 1900) + '-' + to_string(now->tm_mon + 1) + '-' 
                        + to_string(now->tm_mday) + '-' + to_string(now->tm_hour) + '-' 
                        + to_string(now->tm_min) + '-' + to_string(now->tm_sec);
    }
}


void Simulation::output_csv_row(vector <string> outputs) {
    string file_name = output_dir + "/simulation_" + file_suffix + ".csv";
    ofstream file(file_name, ios::app);

    for (string output: outputs) { 
        file << output << ",";
    }

    file << "\n";
    file.close();
}


void Simulation::initialise_csv(BasicEnvironment environment, vector <BasicAgent*> agent_ptrs) {

    //Sets up 1st line for initial settings headers
    vector <string> first_outputs = {"Time Steps", "X-bound", "Y-bound", "# Agents", "# Predators", 
                                     "# Obstacles", "# Energies", "Seek Energy?", "Agent Choice?"};
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
                                      to_string(num_predators), to_string(num_obstacles), to_string(num_energies), 
                                      seek_energy ? "true": "false", to_string(agent_choice)};
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
    for (int i=0; i<num_predators; i++) {
        for (char dimension: vector <char> {'X', 'Y'}) {
            third_outputs.push_back("Predator " + to_string(i+1) + " " + dimension + "-Pos");
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


void Simulation::run_simulation() {

    BasicEnvironment environment(x_bound, y_bound, num_obstacles, num_energies);

    // Creates all predators
    vector <Predator*> predator_ptrs;
    for (int i=0; i<num_predators; i++) {
        string predator_name = "Predator " + to_string(i+1);
        // Needs dynamic allocation to ensure objects survive out of the 'for' scope
        Predator* predator_ptr = new Predator(predator_name, x_bound, y_bound);
        predator_ptrs.push_back(predator_ptr);
    }

    // Creates all agents
    vector <BasicAgent*> agent_ptrs;
    for (int i=0; i<num_agents; i++) {
        string agent_name = "Agent " + to_string(i+1);
        //Creates a new agent pointer from either BasicAgent, AdvancedAgent, or ReplicationAgent, 
        //and uses polymorphism to call each class' respective 'seek_energy()' funtion
        BasicAgent* agent_ptr;
        if (agent_choice == 1) {
            agent_ptr = new BasicAgent(agent_name, x_bound, y_bound);
            cout << "Created BasicAgent..." << endl;
        }
        else if (agent_choice == 2) {
            // Needs dynamic allocation to ensure objects survive out of the 'for' scope
            agent_ptr = new AdvancedAgent(agent_name, x_bound, y_bound);
            cout << "Created AdvancedAgent..." << endl;
        }
        else {
            agent_ptr = new ReplicationAgent(agent_name, min_energy_to_replicate, max_energy_to_replicate, 
                                             max_num_lookaheads, x_bound, y_bound);   
            cout << "Created ReplicationAgent..." << endl;
        }
        agent_ptrs.push_back(agent_ptr);
    }

    // Inserts all predators and agents into the environment and show the initial visualisation (time = 0)
    environment.insert_predators_agents(predator_ptrs, agent_ptrs);
    environment.visualise();

    //Sets up the inital .csv output file w/ 1st line for initial settings headers, 2nd for initial settings, 
    //3rd for other headers, and 4th for time 0 setting
    if (output_csv) {
        initialise_csv(environment, agent_ptrs);
    }

    // For the required number of time steps, have all agents and predators move across the board
    for (int i=0; i<time_steps; i++) {
        //Delays output of new board so shows running in 'real-time' if necessary (and, if the 
        //agents are ReplicationAgents) have them replicate if achieved the required energy
        sleep(time_delay);

        vector <string> outputs;
        for (Predator* predator_ptr: predator_ptrs) {
            predator_ptr->seek_agent();
            outputs.insert(outputs.end(), {to_string(predator_ptr->get_x_pos()), to_string(predator_ptr->get_y_pos())});
        }

        vector <BasicAgent*> new_agent_ptrs;
        bool agent_to_add;
        for (BasicAgent* agent_ptr: agent_ptrs) {
            if (seek_energy) {
                agent_ptr->seek_energy();
            }
            else {
                agent_ptr->move_random();
            }
            //Checks if the underlying object is a ReplicationAgent and, if it is, have it replicate
            if (ReplicationAgent* repl_agent_ptr = dynamic_cast<ReplicationAgent*> (agent_ptr)) {
                bool can_replicate = repl_agent_ptr->check_replicate();
                if (can_replicate) {
                    increment_num_agents();
                    string agent_name = "Agent " + to_string(num_agents);
                    BasicAgent* new_agent_ptr = new ReplicationAgent(agent_name, min_energy_to_replicate, max_energy_to_replicate, 
                                                                     max_num_lookaheads, x_bound, y_bound);
                    new_agent_ptrs.push_back(new_agent_ptr);
                    agent_to_add = true;
                }
            }
            outputs.insert(outputs.end(), {to_string(agent_ptr->get_x_pos()), to_string(agent_ptr->get_y_pos())});
        }

        // If agent(s) have replicated, add them to the new list of active agents only after the current ones have actually moved 
        // (meaning that, on the time instance they spawn, they don't actually move)
        if (agent_to_add) {
            agent_ptrs.insert(agent_ptrs.end(), new_agent_ptrs.begin(), new_agent_ptrs.end());
            environment.insert_predators_agents(predator_ptrs, agent_ptrs);
        }
        
        environment.update();
        environment.visualise();

        //Gets rid of any agents that have been killed by a predator
        vector <BasicAgent*> surviving_agent_ptrs;
        for (BasicAgent* agent_ptr : agent_ptrs) {
            if (!(agent_ptr->get_is_dead())) {
                surviving_agent_ptrs.push_back(agent_ptr);
            }
            else {
                cout << agent_ptr->get_name() << " is dead; removing from the simulation..." << endl;
            }
        }
        agent_ptrs = surviving_agent_ptrs;

        outputs.insert(outputs.begin(), {to_string(i+1), to_string(environment.number_energies_remaining())});

        //Output the simulation state at time 'i+1' to the output .csv
        if (output_csv) {
            output_csv_row(outputs);
        }   
    }


    cout << "Simulation complete!" << endl;
}


void Simulation::increment_num_agents() {
    ++num_agents;
}