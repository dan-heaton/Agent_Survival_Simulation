#ifndef BASICENVIRONMENT_H
#define BASICENVIRONMENT_H

#include <list>
#include <vector>
#include "BasicAgent.h"

using namespace std;


struct Obstacle {
    int x_pos;
    int y_pos;
    Obstacle(int x_pos, int y_pos) :x_pos(x_pos), y_pos(y_pos) {}
};


struct Energy {
    int x_pos;
    int y_pos;
    int energy_val;
    bool is_consumed = false;
    Energy(int x_pos, int y_pos, int energy_val) :x_pos(x_pos), y_pos(y_pos), energy_val(energy_val) {}
};


class BasicEnvironment {
    private:
        list <Obstacle> obstacles;
        list <Energy> energy_sources;
        list <BasicAgent*> agent_ptrs;
        vector <vector <char>> environment_tiles;
        int x_bound;
        int y_bound;
        int time_instance = 0;

        void insert_obstacles_energy(int num_obstacles, int num_energy_sources);

    public:
        BasicEnvironment(int x_bound, int y_bound, int num_obstacles, int num_energy_sources);
        void state_objects();
        void visualise();
        void insert_agent(BasicAgent *agent);
        void update();
};

#endif