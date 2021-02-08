#include <iostream>
#include <string>
#include <algorithm>
#include "../include/BasicAgent.h"

using namespace std;



BasicAgent::BasicAgent(string name, int x_bound, int y_bound) : name(name), x_bound(x_bound), y_bound(y_bound){
    x_pos = rand() % x_bound + 1;
    y_pos = rand() % y_bound + 1;
    cout << "Agent created at (" << x_pos << ", " << y_pos << ")";
    cout << " with name '" << name << "'!" << endl;
}


int BasicAgent::check_path(char direction, int new_pos) {
    if (direction == 'X') {
        // Handles if the agent is moving in the negative direction, so as to properly enter the below 'for' loop
        vector<int> increment_positions;
        for (int i=min(x_pos, new_pos); i<max(x_pos, new_pos)+1; i++) {
            increment_positions.push_back(i);
        }
        if (x_pos > new_pos) {
            reverse(increment_positions.begin(), increment_positions.end());
        }

        for (int i: increment_positions) {
            if (environment_tiles[y_pos-1][i-1] == 'O') {
                cout << name << " encountered obstacle at (" << i << ", " << y_pos << "); stopping here..." << endl;
                // Stops the square before the obstacle
                if (new_pos > x_pos) {
                    return i-1;
                }
                else {
                    return i+1;
                }
            }
            else if (environment_tiles[y_pos-1][i-1] == 'E') {
                cout << name << " encountered energy at (" << i << ", " << y_pos << "); consuming and continuing..." << endl;
                vector <int> energy_position = {y_pos-1, i-1};
                energies_consumed.push_back(energy_position);
                environment_tiles[y_pos-1][i-1] = '-';
            }
        }
    }
    else {
        // Handles if the agent is moving in the negative direction, so as to properly enter the below 'for' loop
        vector<int> increment_positions;
        for (int i=min(y_pos, new_pos); i<max(y_pos, new_pos)+1; i++) {
            increment_positions.push_back(i);
        }
        if (y_pos > new_pos) {
            reverse(increment_positions.begin(), increment_positions.end());
        }

        for (int i: increment_positions) {
            if (environment_tiles[i-1][x_pos-1] == 'O') {
                cout << name << " encountered obstacle at (" << x_pos << ", " << i << "); stopping here..." << endl;
                // Stops the square before the obstacle
                if (new_pos > y_pos) {
                    return i-1;
                }
                else {
                    return i+1;
                }
                
            }
            else if (environment_tiles[i-1][x_pos-1] == 'E') {
                cout << name << " encountered energy at (" << x_pos << ", " << i << "); consuming and continuing..." << endl;
                vector <int> energy_position = {i-1, x_pos-1};
                energies_consumed.push_back(energy_position);
                environment_tiles[i-1][x_pos-1] = '-';
            }
        }
    }
    // If got here, no obstacles in path, so can keep 'new_pos' as final destination on path
    return new_pos;
}


int BasicAgent::check_bounds(int bound, int new_pos) {
    if (new_pos > bound) {
        new_pos = bound;
    }
    else if (new_pos < 1) {
        new_pos = 1;
    }
    return new_pos;
}


void BasicAgent::move(char direction, int move_amount) {
    prev_x_pos = x_pos;
    prev_y_pos = y_pos;
    if (!is_environ_set) {
        cout <<  name << "' can't move if environment is unknown..." << endl;
        return;
    }
    int new_pos;
    if (direction == 'X') {
        new_pos = x_pos + move_amount;
        new_pos = check_bounds(x_bound, new_pos);
    }
    else if (direction == 'Y') {
        new_pos = y_pos + move_amount;
        new_pos = check_bounds(y_bound, new_pos);
    }
    else {
        cout << "Unknown direction '" << direction << "', must be either 'X' or 'Y'..." << endl;
        return;
    }

    // Checks for any obstacles or energy on path; if obstacle found, sets 'new_pos' to location between
    // original X/Y pos and new pos
    new_pos = check_path(direction, new_pos);
    if (direction == 'X') {
        x_pos = new_pos;
    }
    else {
        y_pos = new_pos;
    }

    state_position(direction, move_amount);
}


void BasicAgent::move_random() {
    int move_amount = rand() % (x_bound*2 + 1) - x_bound;
    int direction_num = rand() % 2;


    char direction;
    if (direction_num == 0) {
        direction = 'X';
    }
    else {
        direction = 'Y';
    }
    move(direction, move_amount);
}


vector <vector <int>> BasicAgent::generate_search_positions(int search_radius) {
    //Generates all the positions on the board in which to search for energy
    //Note: positions are relative to current position (centered at (0, 0))
    vector <vector <int>> search_positions;
    for (int i=-search_radius; i<search_radius+1; i++) {
        for (int j=-search_radius; j<search_radius+1; j++) {    
            //Ensures that only 'new' tiles get searched
            if (abs(i) == search_radius or abs(j) == search_radius){
                //Ensures that only tiles in bounds get searched
                if (i+y_pos > 0 and j+x_pos > 0) {
                    vector <int> search_position = {j, i};
                    search_positions.push_back(search_position);
                }
            }   
        }
    }
    return search_positions;
}


bool BasicAgent::move_to_energy(vector <vector <int>> search_positions) {
    //If finds any energies within the selected radius, moves towards it
    //Note: at present, doesn't attempt to move around obstacles (something for more advanced agent)
    bool found_energy = false;
    for (vector<int> search_position: search_positions) {
        int x_search_position = search_position[0] + x_pos - 1;
        int y_search_position = search_position[1] + y_pos - 1;
        //Ensures that agent doesn't try to search out of bounds
        if (y_search_position >= 0 and y_search_position < y_bound and x_search_position >= 0 and x_search_position < x_bound) {
            char tile_value = environment_tiles[y_search_position][x_search_position];
            if (tile_value == 'E') {
                //If energy lies somewhere different on X-plane, move in that direction 
                //and, if not diagonal from the agent, moves onto the energy; otherwise, moves on Y-plane
                if (search_position[0] != 0) {
                    move('X', search_position[0]);
                }
                else {
                    move('Y', search_position[1]);
                }
                found_energy = true;
                return found_energy;
            }
        }
    }
    return found_energy;
}


void BasicAgent::seek_energy() {
    bool found_energy = false;

    //Looks for energy within 'search_radius' tiles and, if none found, expands the search
    int search_radius = 1;
    while (!found_energy) {

        vector <vector <int>> search_positions = generate_search_positions(search_radius);
        found_energy = move_to_energy(search_positions);
        ++search_radius;

        //If the agent can't find any more energy on the board, gives up on seeking energy
        if (search_radius > x_bound and search_radius > y_bound) {
            break;
        }
    }
}


void BasicAgent::state_position(char dimension, int move_size) {
    cout << name << " moved " << move_size << " units on " << dimension << 
    "-axis to new position (" << x_pos << ", " << y_pos << ")" << endl;
}


int BasicAgent::get_x_pos() {
    return x_pos;
}


int BasicAgent::get_y_pos() {
    return y_pos;
}

string BasicAgent::get_name() {
    return name;
}

bool BasicAgent::get_is_dead() {
    return is_dead;
}