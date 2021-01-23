#include "../include/AdvancedAgent.h"

//TODO: get rid of these when 'seek_energy()' properly setup
#include <iostream>
using namespace std;

AdvancedAgent::AdvancedAgent(string message, int x_bound, int y_bound): BasicAgent(message, x_bound, y_bound) {}


void AdvancedAgent::seek_energy() {
    cout << "This is advanced!" << endl;
    BasicAgent::seek_energy();
}