#include <iostream>
#include <random>
#include "../include/ReplicationAgent.h"


using namespace std;


ReplicationAgent::ReplicationAgent(string name, int min_energy, int max_energy, 
                                   int max_lookaheads, int x_bound, int y_bound): 
                                   AdvancedAgent(name, x_bound, y_bound) {

    // Sets these up as object attributes to be used by 'generate_mutations()' further below
    min_energy_to_replicate = min_energy;
    max_energy_to_replicate = max_energy;
    max_num_lookaheads = max_lookaheads;

    speed = rand() % max(x_bound, y_bound) + 1;
    cout << "Agent " << name << " initialised with speed " << speed << "..." << endl;


    energy_to_replicate = rand() % (max_energy_to_replicate - min_energy_to_replicate + 1) + min_energy_to_replicate;
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


bool ReplicationAgent::check_replicate() {
    //If the agent has enough energy to replicate, returns true after subtracting the energy 
    //used to replicate
    int current_energy = get_energy();
    if (current_energy >= energy_to_replicate) {
        set_energy(current_energy - energy_to_replicate);
        cout << "Agent '" << get_name() << "' is replicating..." << endl;
        return true;
    }
    else {
        return false;
    }
}


map <string, int> ReplicationAgent::generate_mutations() {
    //Creates and returns a mapping of inherited values for replicated agents using 
    //normally generated values relative to the replicator agent's values

    default_random_engine generator;

    //Setup w/ mean of 'speed' and S.D. of 'speed/3'; 'speed' closer to 0 than to the x/y-bound 
    //will result in smaller distribution of values for 'new_speed' (still limited to between 0 
    //and 'max_bound') compared to if closer to x/y-bound (the '/3' gives 99% confidence it'll 
    //be above 0, but still need check for this) 
    normal_distribution<double> speed_distribution(speed, speed/3);
    int new_speed;
    bool valid_new_speed = false;
    while (!valid_new_speed) {
        double sample = speed_distribution(generator);
        new_speed = round(sample);
        if (0 < new_speed <= max(x_bound, y_bound)) {
            valid_new_speed = true;
        }
    }

    // Setup so that min energy is within 3 S.D.s (99% confidence) of the mean
    normal_distribution<double> energy_distribution(energy_to_replicate, (energy_to_replicate-min_energy_to_replicate)/3);
    int new_energy_to_replicate;
    bool valid_new_energy = false;
    while (!valid_new_energy) {
        double sample = energy_distribution(generator);
        new_energy_to_replicate = round(sample);
        if (min_energy_to_replicate <= new_energy_to_replicate <= max_energy_to_replicate) {
            valid_new_energy = true;
        }
    }

    // Setup so that 0 lookaheads is within 3 S.D.s (99% confidence) of the mean
    normal_distribution<double> lookaheads_distribution(num_lookaheads, num_lookaheads/3);
    int new_num_lookaheads;
    bool valid_new_lookaheads = false;
    while (!valid_new_lookaheads) {
        double sample = lookaheads_distribution(generator);
        new_num_lookaheads = round(sample);
        if (0 <= new_num_lookaheads <= max_num_lookaheads) {
            valid_new_lookaheads = true;
        }
    }

    map <string, int> mutations_dict;
    mutations_dict["new_speed"] = new_speed;
    mutations_dict["new_energy_to_replicate"] = new_energy_to_replicate;
    mutations_dict["new_num_lookaheads"] = new_num_lookaheads;

    return mutations_dict;
}


void ReplicationAgent::mutate(map<string, int> mutations_dict) {
    //For an agent that's the product of replication, maps from values 
    //generated by a replicator agent to new attributes (i.e. overwrites 
    //the values generated by constructor)

    speed = mutations_dict["new_speed"];
    cout << "New mutated value for 'speed' set to " << speed << "..." << endl;

    energy_to_replicate = mutations_dict["new_energy_to_replicate"];
    cout << "New mutated value for 'energy_to_replicate' set to " << energy_to_replicate << "..." << endl;

    num_lookaheads = mutations_dict["new_num_lookaheads"];
    cout << "New mutated value for 'num_lookaheads' set to " << num_lookaheads << "..." << endl;
}