#include <iostream>
#include <string>
#include "BasicAgent.h"

using namespace std;



BasicAgent::BasicAgent(string message, int x_bound, int y_bound) : x_bound(x_bound), y_bound(y_bound){
    x_pos = rand() % x_bound + 1;
    y_pos = rand() % y_bound + 1;
    cout << "BasicAgent created at (" << x_pos << ", " << y_pos << ")" << endl;
    cout << "BasicAgent says '" << message << "'!" << endl;
}


void BasicAgent::move(char direction, int move_amount) {
    if (!is_environ_set) {
        cout << "Agent can't move if environment is unknown..." << endl;
        return;
    }
    if (direction == 'X') {
        x_pos += move_amount;
        if (x_pos > x_bound) {
            x_pos = x_bound;
        }
    }
    else if (direction == 'Y') {
        y_pos += move_amount;
        if (y_pos > y_bound) {
            y_pos = y_bound;
        }
    }
    else {
        cout << "Unknown direction '" << direction << "', must be either 'X' or 'Y'..." << endl;
        return;
    }
    state_position(direction, move_amount);
}


void BasicAgent::state_position(char dimension, int move_size) {
    cout << "BasicAgent moved " << move_size << " units on " << dimension << 
    "-axis to new position (" << x_pos << ", " << y_pos << ")" << endl;
}
