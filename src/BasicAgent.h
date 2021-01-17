#ifndef BASICAGENT_H
#define BASICAGENT_H

#include <vector>
#include <string>

using namespace std;


class BasicAgent {
    private:
        int x_pos;
        int y_pos;
        int x_bound;
        int y_bound;
        //Unknown until it's initialised into a BasicEnvironment
        vector <vector <char>> environment_tiles;
        bool is_environ_set = false;
        friend class BasicEnvironment;


    public:
        
        BasicAgent(string message, int x_bound=100, int y_bound=100);

        void move(char direction, int move_amount);
        void state_position(char dimension, int move_size);
};

#endif