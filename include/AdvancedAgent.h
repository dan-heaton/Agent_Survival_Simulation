#ifndef ADVANCEDAGENT_H
#define ADVANCEDAGENT_H

#include <map>
#include "../include/BasicAgent.h"


class AdvancedAgent: public BasicAgent {
    private:
        //Stores a mental map of directions that are blocked and how far away they are that is reset at the beginning of each move
        map <char, int> blocked_directions;
        void find_blocked_directions(vector <vector <int>> search_positions);
        vector <vector <int>> remove_search_positions(vector <vector <int>> search_positions);

    public:
        AdvancedAgent(string message, int x_bound=100, int y_bound=100);
        void seek_energy();
};



#endif