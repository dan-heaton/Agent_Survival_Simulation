#ifndef PREDATOR_H
#define PREDATOR_H

#include <string>
#include "BasicAgent.h"


class Predator: public BasicAgent {
    private:
        int speed;
        vector <vector <int>> find_agents();

    public:
        Predator(string message, int x_bound=100, int y_bound=100);
        void seek_agent();
};

#endif