#ifndef PREDATOR_H
#define PREDATOR_H

#include <string>
#include "BasicAgent.h"


class Predator {
    private:
        string name;
        int x_pos;
        int y_pos;
        int x_bound;
        int y_bound;
        //Unknown until it's initialised into a BasicEnvironment
        vector <vector <char>> environment_tiles;
        int speed;
        friend class BasicEnvironment;
        vector <vector <int>> generate_search_positions();
        int check_bounds(int bound, int new_pos);
        bool move_to_agent(vector <vector <int>> search_positions);

    public:
        Predator(string message, int x_bound=100, int y_bound=100);
        void seek_agent();
        int get_x_pos();
        int get_y_pos();
        string get_name();
};

#endif