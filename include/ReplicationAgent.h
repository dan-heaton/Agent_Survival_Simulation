#ifndef REPLICATIONAGENT_H
#define REPLICATIONAGENT_H

#include "../include/AdvancedAgent.h"


class ReplicationAgent: public AdvancedAgent {

    private:
        int speed;
        int energy_to_replicate;

    public:
        ReplicationAgent(string name, int max_energy_to_replicate, int x_bound=100, int y_bound=100);
        void replicate();
        void seek_energy();
};


#endif