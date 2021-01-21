#ifndef BASICAGENT_H
#define BASICAGENT_H

#include <vector>
#include <string>

using namespace std;


class BasicAgent {
    private:
        string name;
        int x_pos;
        int y_pos;
        int x_bound;
        int y_bound;
        int energy = 0;
        //Unknown until it's initialised into a BasicEnvironment
        vector <vector <char>> environment_tiles;
        vector <vector <int>> energies_consumed;
        bool is_environ_set = false;
        friend class BasicEnvironment;

        int check_path(char direction, int new_pos);
        int check_bounds(int bound, int new_pos);

    public:  
        BasicAgent(string message, int x_bound=100, int y_bound=100);
        void move(char direction, int move_amount);
        void move_random();
        void seek_energy();
        void state_position(char dimension, int move_size);
        int get_x_pos();
        int get_y_pos();
};

#endif