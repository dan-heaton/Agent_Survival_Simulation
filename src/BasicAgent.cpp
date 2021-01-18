#include <iostream>
#include <string>
#include "BasicAgent.h"

using namespace std;



BasicAgent::BasicAgent(string name, int x_bound, int y_bound) : name(name), x_bound(x_bound), y_bound(y_bound){
    x_pos = rand() % x_bound + 1;
    y_pos = rand() % y_bound + 1;
    cout << "BasicAgent created at (" << x_pos << ", " << y_pos << ")";
    cout << " with name '" << name << "'!" << endl;
}


int BasicAgent::check_path(char direction, int new_pos) {
    if (direction == 'X') {
        for (int i=x_pos; i<new_pos; i++) {
            if (environment_tiles[y_pos-1][i-1] == 'O') {
                cout << "Agent '" << name << "' encountered obstacle at (" << i << ", " << y_pos << "); stopping here..." << endl;
                // Stops the square before the obstacle
                return i-1;
            }
            else if (environment_tiles[y_pos-1][i-1] == 'E') {
                cout << "Agent '" << name << "' encountered energy at (" << i << ", " << y_pos << "); consuming and continuing..." << endl;
                energy += 1;
                environment_tiles[y_pos-1][i-1] = '-';
            }
        }
    }
    else {
        for (int i=y_pos; i<new_pos; i++) {
            if (environment_tiles[i-1][x_pos-1] == 'O') {
                cout << "Agent '" << name << "' encountered obstacle at (" << x_pos << ", " << i << "); stopping here..." << endl;
                // Stops the square before the obstacle
                return i-1;
            }
            else if (environment_tiles[i-1][x_pos-1] == 'E') {
                cout << "Agent '" << name << "' encountered energy at (" << x_pos << ", " << i << "); consuming and continuing..." << endl;
                energy += 1;
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
    else if (new_pos < 0) {
        new_pos = 0;
    }
    return new_pos;
}


void BasicAgent::move(char direction, int move_amount) {
    if (!is_environ_set) {
        cout << "Agent '" << name << "' can't move if environment is unknown..." << endl;
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


void BasicAgent::state_position(char dimension, int move_size) {
    cout << "BasicAgent '"<<  name << "' moved " << move_size << " units on " << dimension << 
    "-axis to new position (" << x_pos << ", " << y_pos << ")" << endl;
}
