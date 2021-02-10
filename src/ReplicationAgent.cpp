#include <iostream>
#include "../include/ReplicationAgent.h"


using namespace std;


ReplicationAgent::ReplicationAgent(string name, int max_energy_to_replicate, int x_bound, int y_bound): AdvancedAgent(name, x_bound, y_bound) {
    speed = rand() % max(x_bound, y_bound) + 1;
    cout << "Agent " << name << " initialised with speed " << speed << "..." << endl;


    energy_to_replicate = rand() % max_energy_to_replicate + 1;
    cout << "Agent " << name << " initialised with energy to replicate being " << energy_to_replicate << "..." << endl;
};


void ReplicationAgent::find_outbounds_directions(vector <vector <int>> search_positions) {
    for (vector <int> search_position: search_positions) {
        int x_search_position = search_position[0] + get_x_pos() - 1;
        int y_search_position = search_position[1] + get_y_pos() - 1;
        if (x_search_position < 0) {
            blocked_directions['W'] = search_position[0];
        }
        if (x_search_position >= x_bound) {
            blocked_directions['E'] = search_position[0];
        }
        if (y_search_position < 0) {
            blocked_directions['N'] = search_position[1];
        }
        if (y_search_position >= y_bound) {
            blocked_directions['S'] = search_position[1];
        }
    }
}


void ReplicationAgent::seek_energy() {

    //Resets 'blocked_directions' so agent will explore all avenues from current position
    blocked_directions.clear();

    //If determined to be in a loop (i.e. same position as previous time 'seek_energy()' was called), 
    //forcefully breaks out by moving in a random direction and magnitude
    vector <int> current_position = {get_x_pos(), get_y_pos()};
    if (previous_positions.size() > 0) {
        if (equal(previous_positions.back().begin(), previous_positions.back().end(), current_position.begin())) {
            break_out_loop(current_position);
            //The above does the move for the turn, so don't want to move again on same 'seek_energy()' call
            return;
        }
    }
    previous_positions.push_back(current_position);

    //Only search for energies within the 'reach' of the agent
    for (int i=1; i<speed+1; i++) {
        vector <vector <int>> search_positions = generate_search_positions(i);

        //Limits new positions to search to being only those which aren't blocked by an obstacle 
        //or that aren't out of bounds
        find_blocked_directions(search_positions);
        find_outbounds_directions(search_positions);

        search_positions = remove_search_positions(search_positions);
        bool found_energy = move_to_energy(search_positions);

        //If already found an energy and moved to it, break out of loop as no need to look/move further
        if (found_energy) {
            break;
        }
    }

}


void ReplicationAgent::replicate() {
    //TODO : add this!
}