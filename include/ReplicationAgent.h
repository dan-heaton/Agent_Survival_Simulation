#ifndef REPLICATIONAGENT_H
#define REPLICATIONAGENT_H

#include "../include/AdvancedAgent.h"


class ReplicationAgent: public AdvancedAgent {

    private:
        int speed;
        int energy_to_replicate;
        void find_outbounds_directions(vector <vector <int>> search_positions);

    public:
        ReplicationAgent(string name, int max_energy_to_replicate, int x_bound=100, int y_bound=100);
        void seek_energy();
        void replicate();
};


#endif