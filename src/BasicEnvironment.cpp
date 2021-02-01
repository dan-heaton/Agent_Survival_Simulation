#include <iostream>
#include <vector>
#include "../include/BasicEnvironment.h"

using namespace std;


const int MAX_ENERGY_VAL = 100;


void BasicEnvironment::insert_obstacles_energy(int num_obstacles, int num_energy_sources) {
    // Creates 'num_obstacles' objects at random positions within the environment bounds, and inserts them into the 2D board
    for (int i=0; i<num_obstacles; i++) {
        bool inserted_obstacle = false;

        //Ensures that the obstacle is placed in a 'new' position (i.e. not on top of any other obstacle)
        while (!inserted_obstacle) {
            int x_pos = rand() % x_bound + 1;
            int y_pos = rand() % y_bound + 1;
            if (environment_tiles[y_pos-1][x_pos-1] == '-') {
                Obstacle obst(x_pos, y_pos);
                obstacles.push_back(obst);
                environment_tiles[y_pos-1][x_pos-1] = 'O';
                inserted_obstacle = true;
            }
        }
    }

    // Creates 'num_energy_sources' objects at random positions within the environment bounds, and inserts them into the 2D board
    for (int i=0; i<num_energy_sources; i++) {
        bool inserted_energy = false;

        //Ensures that the energy is placed in a 'new' position (i.e. not on top of any other obstacle/energy)
        while (!inserted_energy) {
            int x_pos = rand() % x_bound + 1;
            int y_pos = rand() % y_bound + 1;
            int energy_val = rand() % MAX_ENERGY_VAL + 1;
            if (environment_tiles[y_pos-1][x_pos-1] == '-') {
                Energy ener(x_pos, y_pos, energy_val);
                energy_sources.push_back(ener);
                environment_tiles[y_pos-1][x_pos-1] = 'E';
                inserted_energy = true;
            }
        }
    }
}


BasicEnvironment::BasicEnvironment(int x_bound, int y_bound, int num_obstacles, int num_energy_sources) 
                        :x_bound(x_bound), y_bound(y_bound) {

    // Initalises all environment tiles as empty spaces ("-") based on supplied dimensions, 
    // before obstacles or energy sources are added
    for (int i=0; i<y_bound; i++) {
        vector <char> environment_tiles_row;
        for (int j=0; j<x_bound; j++) {
            environment_tiles_row.push_back('-');
        }
        environment_tiles.push_back(environment_tiles_row);
    }
    insert_obstacles_energy(num_obstacles, num_energy_sources);
}


void BasicEnvironment::state_objects() {
    for (Obstacle obst: obstacles) {
        cout << "Obstacle at (" << obst.x_pos << ", " << obst.y_pos << ")" << endl;
    }
    for (Energy ener: energy_sources) {
        cout << "Energy source at (" << ener.x_pos << ", " << ener.y_pos << ") with energy value " << ener.energy_val << endl;
    }
}


void BasicEnvironment::visualise(){
    cout << "Time: " << time_instance << endl;
    for (int i=0; i<environment_tiles.size(); i++) {
        for (int j=0; j<environment_tiles[i].size(); j++) {
            cout << " " << environment_tiles[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}
        
void BasicEnvironment::insert_agent(BasicAgent *agent){
    agent_ptrs.push_back(agent);
    environment_tiles[agent->y_pos - 1][agent->x_pos - 1] = 'A';

    // Updates each agent's knowledge of environment when a new agent is inserted into one
    for (BasicAgent *agnt: agent_ptrs) {
        agnt->environment_tiles = environment_tiles;
        agnt->is_environ_set = true;
    }
}


void BasicEnvironment::insert_predator(Predator *predator){
    predator_ptrs.push_back(predator);
    environment_tiles[predator->y_pos - 1][predator->x_pos - 1] = 'P';

    // Updates each agent's knowledge of environment when a new agent is inserted into one
    for (Predator *pred: predator_ptrs) {
        pred->environment_tiles = environment_tiles;
        pred->is_environ_set = true;
    }
}


void BasicEnvironment::update() {

    time_instance += 1;

    // Resets any previous agent icon on tiles from 'A' or 'P' to '-'
    for (int i=0; i<environment_tiles.size(); i++) {
        for (int j=0; j<environment_tiles[i].size(); j++) {
            if (environment_tiles[i][j] == 'A' or environment_tiles[i][j] == 'P') {
                environment_tiles[i][j] = '-';
            }
        }
    }

    // Adds the new agents onto the tiles
    for (BasicAgent *agnt: agent_ptrs) {
        environment_tiles[agnt->y_pos-1][agnt->x_pos-1] = 'A';

        // For any energies the agent has recently consumed, update their own energy value 
        // and gets rid of the energy on the board
        for (vector<int> energy_consumed: agnt->energies_consumed) {
            for (Energy &energy_source: energy_sources) {
                if (energy_source.x_pos-1 == energy_consumed[1] and energy_source.y_pos-1 == energy_consumed[0] and !energy_source.is_consumed) {
                    agnt->energy += energy_source.energy_val;
                    cout << "Agent '" << agnt->name << "' received " << energy_source.energy_val << " energy; now has " << agnt->energy << " energy" << endl;
                    // Ensures that if the agent has stopped on the location of the energy we don't overwrite it's own icon on the tiles
                    if (environment_tiles[energy_source.y_pos-1][energy_source.x_pos-1] != 'A') {
                        environment_tiles[energy_source.y_pos-1][energy_source.x_pos-1] = '-';
                    }
                    energy_source.is_consumed = true;
                    break;
                }
            }
        }
        agnt->energies_consumed.clear();
    }
    // Adds the new predators onto the tiles
    for (Predator *pred: predator_ptrs) {
        if (environment_tiles[pred->y_pos-1][pred->x_pos-1] == 'A') {
            cout << "Predator '" << pred->name << "' found agent at (" << pred->x_pos << ", " << pred->y_pos << ") and has eaten it!" << endl;
            // Finds the agent which has been eaten and set it to be dead
            for (BasicAgent *agnt: agent_ptrs) {
                if (agnt->x_pos == pred->x_pos and agnt->y_pos == pred->y_pos) {
                    agnt->is_dead = true;
                }
            }
        }
        environment_tiles[pred->y_pos-1][pred->x_pos-1] = 'P';
    }
}


int BasicEnvironment::number_energies_remaining() {
    int num_remaining = 0;
    for (Energy energy_source: energy_sources) {
        if (!energy_source.is_consumed) {
            ++num_remaining;
        }
    }
    return num_remaining;
}


list <Obstacle> BasicEnvironment::get_obstacles() {
    return obstacles;
}


list <Energy> BasicEnvironment::get_energy_sources() {
    return energy_sources;
}