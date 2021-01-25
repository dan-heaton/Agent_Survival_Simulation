#ifndef ADVANCEDAGENT_H
#define ADVANCEDAGENT_H

#include "../include/BasicAgent.h"


class AdvancedAgent: public BasicAgent {
    private:
        vector <char> find_blocked_directions(vector <vector <int>> search_positions);
        vector <vector <int>> remove_search_positions(vector <vector <int>> search_positions, vector<char> blocked_directions);

    public:
        AdvancedAgent(string message, int x_bound=100, int y_bound=100);
        void seek_energy();
};



#endif