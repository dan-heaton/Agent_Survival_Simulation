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
        //Used to determine when caught by predator
        int prev_x_pos;
        int prev_y_pos;
        int energy = 0;
        vector <vector <int>> energies_consumed;
        bool is_environ_set = false;
        bool is_dead = false;
        friend class BasicEnvironment;
        void state_position(char dimension, int move_size);
        int check_path(char direction, int new_pos);
        int check_bounds(int bound, int new_pos);

    protected:
        //Below needs to be seen by 'AdvancedAgent' when redefining 'seek_energy()'
        int x_bound;
        int y_bound;
        //Unknown until it's initialised into a BasicEnvironment
        vector <vector <char>> environment_tiles;
        vector <vector <int>> generate_search_positions(int search_radius);
        bool move_to_energy(vector <vector <int>> search_positions);

    public:  
        BasicAgent(string name, int x_bound=100, int y_bound=100);
        void move(char direction, int move_amount);
        void move_random();
        //Virtual allows AdvancedAgent's overriden version to be used correctly in Simulation class
        virtual void seek_energy();
        int get_x_pos();
        int get_y_pos();
        string get_name();
        bool get_is_dead();
};


#endif