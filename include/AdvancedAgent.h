#ifndef ADVANCEDAGENT_H
#define ADVANCEDAGENT_H

#include "../include/BasicAgent.h"


class AdvancedAgent: public BasicAgent {
    public:
        AdvancedAgent(string message, int x_bound=100, int y_bound=100);
        void seek_energy();
};



#endif