#include <algorithm>
#include <iostream>
#include "../include/AdvancedAgent.h"

using namespace std;


AdvancedAgent::AdvancedAgent(string name, int x_bound, int y_bound): BasicAgent(name, x_bound, y_bound) {}


void AdvancedAgent::find_blocked_directions(vector <vector <int>> search_positions) {
    for (vector <int> search_position: search_positions) {
        int x_search_position = search_position[0] + get_x_pos() - 1;
        int y_search_position = search_position[1] + get_y_pos() - 1;
        //Ensures that agent doesn't try to search out of bounds
        if (y_search_position >= 0 and y_search_position < y_bound and x_search_position >= 0 and x_search_position < x_bound) {
            char tile_value = environment_tiles[y_search_position][x_search_position];
            if (tile_value == 'O') {
                //Note: 'find's are used below to ensure that only the closest blocked position is stored as a value (though doesn't count '0' as 
                //this is inserted as default value by compiler)
                if (search_position[0] == 0) {
                    if (search_position[1] < 0 and (blocked_directions.find('N') == blocked_directions.end() or blocked_directions['N'] == 0)) {
                        blocked_directions['N'] = search_position[1];
                    }
                    else if (search_position[1] > 0 and (blocked_directions.find('S') == blocked_directions.end() or blocked_directions['S'] == 0)) {
                        blocked_directions['S'] = search_position[1];
                    }
                }
                else if (search_position[1] == 0) {
                    if (search_position[0] < 0 and (blocked_directions.find('W') == blocked_directions.end() or blocked_directions['W'] == 0)) {
                        blocked_directions['W'] = search_position[0];
                    }
                    else if(search_position[0] > 0 and (blocked_directions.find('E') == blocked_directions.end() or blocked_directions['E'] == 0)) {
                        blocked_directions['E'] = search_position[0];
                    }
                }
            }
        }
    }
}


vector <vector <int>> AdvancedAgent::remove_search_positions(vector <vector <int>> search_positions) {
    //Clears out any blocked directions or any searches in diagonal directions from the list of 'search_positions', 
    //so we are left with the valid directions (relative to current position) in which we can search
    vector <vector <int>> new_search_positions;
    for (vector <int> search_position: search_positions) {
        bool north_is_blocked = !(blocked_directions.find('N') == blocked_directions.end()) and blocked_directions['N'] !=0 
                                and search_position[1] <= blocked_directions['N'];
        if (!(search_position[0] == 0 and search_position[1] < 0 and north_is_blocked)) {
            bool south_is_blocked = !(blocked_directions.find('S') == blocked_directions.end()) and blocked_directions['S'] != 0 
                                    and search_position[1] >= blocked_directions['S'];
            if (!(search_position[0] == 0 and search_position[1] > 0 and south_is_blocked)) {
                bool west_is_blocked = !(blocked_directions.find('W') == blocked_directions.end()) and blocked_directions['W'] != 0 
                                       and search_position[0] <= blocked_directions['W'];
                if (!(search_position[1] == 0 and search_position[0] < 0 and west_is_blocked)) {
                    bool east_is_blocked = !(blocked_directions.find('E') == blocked_directions.end()) and blocked_directions['E'] != 0 
                                           and search_position[0] >= blocked_directions['E'];
                    if (!(search_position[1] == 0 and search_position[0] > 0 and east_is_blocked)) {
                        //Ensures only searching in one of the cardinal directions, i.e. no diagonals
                        if (search_position[0] == 0 or search_position[1] == 0) {
                            new_search_positions.push_back(search_position);
                        }
                    }
                }
            }
        } 
    }
    return new_search_positions;
}


void AdvancedAgent::break_out_loop(vector <int> current_position) {

    bool have_moved = false;
    while (!have_moved) {
        int previous_x_pos = get_x_pos();
        int previous_y_pos = get_y_pos();
        move_random();
        //Ensures that we keep trying to make a random move even if current one is blocked
        if (previous_x_pos != get_x_pos() or previous_y_pos != get_y_pos()) {
            have_moved = true;
        }
    }
    previous_positions.push_back(current_position);
}


void AdvancedAgent::seek_energy() {
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


    bool found_energy = false;
    //Looks for energy within 'search_radius' tiles and, if none found, expands the search
    int search_radius = 1;
    while (!found_energy) {

        vector <vector <int>> search_positions = generate_search_positions(search_radius);

        //Limits new positions to search to being only those which aren't blocked by an obstacle
        find_blocked_directions(search_positions);
        search_positions = remove_search_positions(search_positions);

        found_energy = move_to_energy(search_positions);
        ++search_radius;

        //If the agent can't find any more energy on the board, gives up on seeking energy
        if (search_radius >= x_bound and search_radius >= y_bound) {
            break;
        }
    }
}