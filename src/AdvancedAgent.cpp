#include "../include/AdvancedAgent.h"

//TODO: get rid of these when 'seek_energy()' properly setup
#include <algorithm>
using namespace std;

AdvancedAgent::AdvancedAgent(string message, int x_bound, int y_bound): BasicAgent(message, x_bound, y_bound) {}


vector <char> AdvancedAgent::find_blocked_directions(vector <vector <int>> search_positions) {
    vector <char> blocked_directions;
    for (vector <int> search_position: search_positions) {
        int x_search_position = search_position[0] + get_x_pos() - 1;
        int y_search_position = search_position[1] + get_y_pos() - 1;
        //Ensures that agent doesn't try to search out of bounds
        if (y_search_position >= 0 and y_search_position < y_bound and x_search_position >= 0 and x_search_position < x_bound) {
            char tile_value = environment_tiles[y_search_position][x_search_position];
            if (tile_value == 'O') {
                if (search_position[0] == 0) {
                    if (search_position[1] == -1) {
                        blocked_directions.push_back('N');
                    }
                    else if (search_position[1] == 1) {
                        blocked_directions.push_back('S');
                    }
                }
                else if (search_position[1] == 0) {
                    if (search_position[0] == -1) {
                        blocked_directions.push_back('W');
                    }
                    else if(search_position[0] == 1) {
                        blocked_directions.push_back('E');
                    }
                }
            }
        }
    }
    return blocked_directions;
}


vector <vector <int>> AdvancedAgent::remove_search_positions(vector <vector <int>> search_positions, vector<char> blocked_directions) {
    vector <vector <int>> new_search_positions;
    for (vector <int> search_position: search_positions) {
        if (!(search_position[0] == 0 and search_position[1] < 0 and count(blocked_directions.begin(), blocked_directions.end(), 'N'))) {
            if (!(search_position[0] == 0 and search_position[1] > 0 and count(blocked_directions.begin(), blocked_directions.end(), 'S'))) {
                if (!(search_position[1] == 0 and search_position[0] < 0 and count(blocked_directions.begin(), blocked_directions.end(), 'W'))) {
                    if (!(search_position[1] == 0 and search_position[0] > 0 and count(blocked_directions.begin(), blocked_directions.end(), 'E'))) {
                        new_search_positions.push_back(search_position);
                    }
                }
            }
        } 
    }
    return new_search_positions;
}


void AdvancedAgent::seek_energy() {
    bool found_energy = false;

    //Looks for energy within 'search_radius' tiles and, if none found, expands the search
    int search_radius = 1;
    vector <char> blocked_directions;
    while (!found_energy) {

        vector <vector <int>> search_positions = generate_search_positions(search_radius);
        vector <char> new_blocked_directions = find_blocked_directions(search_positions);
        //Ensures that existing blocked directions from smaller value of 'search_radius' is still valid at larger values 
        //(note: may have duplicate directions which is fine)
        blocked_directions.insert(blocked_directions.end(), new_blocked_directions.begin(), new_blocked_directions.end());
        //Limits new positions to search to being only those which aren't blocked by an obstacle
        search_positions = remove_search_positions(search_positions, blocked_directions);
        found_energy = move_to_energy(search_positions);
        ++search_radius;

        //If the agent can't find any more energy on the board, gives up on seeking energy
        if (search_radius > x_bound and search_radius > y_bound) {
            break;
        }
    }
}