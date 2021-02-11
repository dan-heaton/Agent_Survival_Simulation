#include <iostream>
#include "../include/ReplicationAgent.h"


using namespace std;


ReplicationAgent::ReplicationAgent(string name, int max_energy_to_replicate, int max_num_lookaheads, int x_bound, int y_bound): 
                                   AdvancedAgent(name, x_bound, y_bound) {
    speed = rand() % max(x_bound, y_bound) + 1;
    cout << "Agent " << name << " initialised with speed " << speed << "..." << endl;


    energy_to_replicate = rand() % max_energy_to_replicate + 1;
    cout << "Agent " << name << " initialised with energy to replicate being " << energy_to_replicate << "..." << endl;

    num_lookaheads = rand() % max_num_lookaheads + 1;
    cout << "Agent " << name << " initialised with number of lookaheads allowed to being " << num_lookaheads << "..." << endl;
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


void ReplicationAgent::lookahead_reset(int original_x_pos, int original_y_pos, vector <vector <char>> original_environment_tiles, 
                                       map <char, int> original_blocked_directions, vector <vector <int>> original_energies_consumed) {
    //Resets the position and view of the agent to original values that may have 
    //been manipulated during lookahead speculations
    set_x_pos(original_x_pos);
    set_y_pos(original_y_pos);
    environment_tiles = original_environment_tiles;
    blocked_directions = original_blocked_directions;
    energies_consumed = original_energies_consumed;
}


vector <int> ReplicationAgent::lookahead(vector <vector <int>> search_positions, int num_lookaheads) {

    //Stores the actual values of the agent, which will be manipulated during lookaheads 
    //as the agent searches for where it may be in the future
    int original_x_pos = get_x_pos();
    int original_y_pos = get_y_pos();
    vector <vector <char>> original_environment_tiles = environment_tiles;
    map <char, int> original_blocked_directions = blocked_directions;
    vector <vector <int>> original_energies_consumed = energies_consumed;

    //Breadth first search below, as want to prioritise positions to move to that need fewer moves

    // Attempts to find any new positions to move to from the current set 
    // of positions (i.e. looking ahead)
    vector <vector <vector <int>>> layer_inner_search_positions;
    for (vector <int> search_position: search_positions) {
        set_x_pos(search_position[0] + get_x_pos());
        set_y_pos(search_position[1] + get_y_pos());
        blocked_directions.clear();
        for (int i=1; i<speed+1; i++) {
            vector <vector <int>> inner_search_positions = generate_search_positions(i);

            find_blocked_directions(inner_search_positions);
            find_outbounds_directions(inner_search_positions);

            inner_search_positions = remove_search_positions(inner_search_positions);
            layer_inner_search_positions.push_back(inner_search_positions);

            bool found_energy = move_to_energy(inner_search_positions);
            lookahead_reset(original_x_pos, original_y_pos, environment_tiles, 
                            original_blocked_directions, original_energies_consumed);
            if (found_energy) {
                vector <int> move_pos = search_position;
                return move_pos;
            }
        }
        lookahead_reset(original_x_pos, original_y_pos, environment_tiles, 
                        original_blocked_directions, original_energies_consumed);
    }

    // At this point, the agent is unable to find a new position to move to by looking one 'layer' 
    // of positions into the future, so recursively attempts this on future layers (moving to next 
    // layer of tree search) if allowed by 'num_lookaheads
    for (vector <vector <int>> inner_search_positions: layer_inner_search_positions) {
        if (num_lookaheads > 1) {
            vector <int> move_pos = lookahead(inner_search_positions, num_lookaheads-1);
            if (move_pos != vector <int> {get_x_pos(), get_y_pos()}) {
                return move_pos;
            }
        }
    }

    lookahead_reset(original_x_pos, original_y_pos, environment_tiles, 
                    original_blocked_directions, original_energies_consumed);

    //If got here, couldn't find any positions to move to via lookahead, 
    //so chooses to remain stationary
    vector <int> move_pos = {get_x_pos(), get_y_pos()};
    return move_pos;

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
    bool found_energy;
    vector <vector <int>> total_search_positions;
    for (int i=1; i<speed+1; i++) {
        vector <vector <int>> search_positions = generate_search_positions(i);

        //Limits new positions to search to being only those which aren't blocked by an obstacle 
        //or that aren't out of bounds
        find_blocked_directions(search_positions);
        find_outbounds_directions(search_positions);

        search_positions = remove_search_positions(search_positions);
        total_search_positions.insert(total_search_positions.end(), search_positions.begin(), search_positions.end());
        found_energy = move_to_energy(search_positions);

        //If already found an energy and moved to it, break out of loop as no need to look/move further
        if (found_energy) {
            break;
        }
    }

    //If agent hasn't found any energy yet, attempts to find a new position the agent would 
    //rather be in and moves to that position
    if (!(found_energy)) {
        cout << "-------STARTING LOOKAHEAD (speculations below; not real moves)-------" << endl;
        vector <int> move_pos = lookahead(total_search_positions, num_lookaheads);
        cout << "-------ENDING LOOKAHEAD (ended speculations)-------" << endl;
        if (move_pos[0] != 0) {
            move('X', move_pos[0]);
        }
        else if (move_pos[1] != 0) {
            move('Y', move_pos[1]);
        }
    }
}


void ReplicationAgent::replicate() {
    //TODO : add this!
}