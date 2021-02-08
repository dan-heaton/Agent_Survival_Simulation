#include <iostream>
#include "../include/ReplicationAgent.h"


using namespace std;


ReplicationAgent::ReplicationAgent(string name, int max_energy_to_replicate, int x_bound, int y_bound): AdvancedAgent(name, x_bound, y_bound) {
    speed = rand() % max(x_bound, y_bound) + 1;
    cout << "Agent " << name << " initialised with speed " << speed << "..." << endl;


    energy_to_replicate = rand() % max_energy_to_replicate + 1;
    cout << "Agent " << name << " initialised with energy to replicate being " << energy_to_replicate << "..." << endl;
};


void ReplicationAgent::replicate() {
    //TODO : add this!
}


void ReplicationAgent::seek_energy() {
    //TODO: add this as overriden from AdvancedAgent to take value of energy into account and undertake 'lookahead' search
}