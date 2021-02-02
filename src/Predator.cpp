#include <iostream>
#include "../include/Predator.h"

using namespace std;


Predator::Predator(string message, int x_bound, int y_bound): BasicAgent(message, x_bound, y_bound) {
    //speed = rand() % max(x_bound, y_bound) + 1;
    
    // TODO : revert this!
    speed = 20;
    cout << "Predator initialised with speed " << speed << "..." << endl;
}


void Predator::seek_agent() {
    vector <vector <int>> search_positions;
    for (int i=0; i<speed; i++) {
        vector <vector <int>> new_search_positions = generate_search_positions(i);
        //Remove any search positions that are diagonal directions
        vector <vector <int>> filtered_search_positions;
        for (vector <int> search_position: new_search_positions) {
            if (search_position[0] == 0 or search_position[1] == 0) {
                filtered_search_positions.push_back(search_position);
            }
        }
        search_positions.insert(search_positions.end(), filtered_search_positions.begin(), filtered_search_positions.end());
    }
    int previous_x_pos = get_x_pos();
    int previous_y_pos = get_y_pos();
    //Currently, if it can't see an agent, stays in place
    bool found_agent = move_to_target(search_positions, 'A');
}