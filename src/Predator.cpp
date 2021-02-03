#include <iostream>
#include "../include/Predator.h"

using namespace std;


Predator::Predator(string name, int x_bound, int y_bound): name(name), x_bound(x_bound), y_bound(y_bound) {
    x_pos = rand() % x_bound + 1;
    y_pos = rand() % y_bound + 1;
    cout << "Predator created at (" << x_pos << ", " << y_pos << ")";
    cout << " with name '" << name << "'!" << endl;
    //speed = rand() % max(x_bound, y_bound) + 1;
    
    // TODO : revert this!
    speed = 20;
    cout << "Predator initialised with speed " << speed << "..." << endl;
}


vector <vector <int>> Predator::generate_search_positions() {
    //Generates and returns a list of valid positions that the predator can search (assuming it can 
    //hop over obstacles and energy without consuming them along the way)
    vector <vector <int>> search_positions;

    for (int i=-speed; i<(speed+1); i++) {
        // Below 'if' statement prevents (0, 0) being added as search positions
        if (i != 0) {
            if (x_pos + i >= 1 and x_pos + i <= x_bound) {
                search_positions.push_back(vector <int> {i, 0});
            }
            if (y_pos + i >= 1 and y_pos + i <= y_bound) {
                search_positions.push_back(vector <int> {0, i});
            }
        }
    }
    return search_positions;
}


int Predator::check_bounds(int bound, int new_pos) {
    if (new_pos > bound) {
        new_pos = bound;
    }
    else if (new_pos < 1) {
        new_pos = 1;
    }
    return new_pos;
}


bool Predator::move_to_agent(vector <vector <int>> search_positions) {
    //If finds any agents within the selected radius, goes to eat it!
    bool found_agent = false;
    for (vector<int> search_position: search_positions) {
        int x_search_position = search_position[0] + x_pos - 1;
        int y_search_position = search_position[1] + y_pos - 1;
        //Ensures that predator doesn't try to search out of bounds
        if (y_search_position >= 0 and y_search_position < y_bound and x_search_position >= 0 and x_search_position < x_bound) {
            char tile_value = environment_tiles[y_search_position][x_search_position];
            if (tile_value == 'A') {
                //If agent lies somewhere different on X-plane, move in that direction; otherwise, moves on Y-plane
                if (search_position[0] != 0) {
                    x_pos += search_position[0];
                    x_pos = check_bounds(x_bound, x_pos);
                }
                else {
                    y_pos += search_position[1];
                    y_pos = check_bounds(y_bound, y_pos);
                }
                found_agent = true;
                return found_agent;
            }
        }
    }
    return found_agent;
}


void Predator::seek_agent() {
    
    vector <vector <int>> search_positions = generate_search_positions();
    bool found_agent = move_to_agent(search_positions);
    // If unable to capture an agent, moves 1 unit randomly in any either direction
    if (!found_agent) {
        int direction_num = rand() % 2;
        if (direction_num == 0) {
            x_pos = check_bounds(x_bound, x_pos+1);
        }
        else {
            y_pos = check_bounds(y_bound, y_pos+1);
        }
    }
}


int Predator::get_x_pos() {
    return x_pos;
}


int Predator::get_y_pos() {
    return y_pos;
}

string Predator::get_name() {
    return name;
}