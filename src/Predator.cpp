#include <iostream>
#include "../include/Predator.h"

using namespace std;


Predator::Predator(string message, int x_bound, int y_bound): BasicAgent(message, x_bound, y_bound) {}


vector <vector <int>> Predator::find_agents() {}


void Predator::seek_agent() {
    // TODO: make sure to utilise the 'speed' attribute (which needs to be randomised with constructor)
    move_random();
}