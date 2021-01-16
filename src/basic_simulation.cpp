#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

const int MAX_ENERGY_VAL = 100;
const int X_BOUND = 15;
const int Y_BOUND = 15;


struct Obstacle {
    int x_pos;
    int y_pos;
    Obstacle(int x_pos, int y_pos) :x_pos(x_pos), y_pos(y_pos) {}
};


struct Energy {
    int x_pos;
    int y_pos;
    int energy_val;
    Energy(int x_pos, int y_pos, int energy_val) :x_pos(x_pos), y_pos(y_pos), energy_val(energy_val) {}
};


class BasicAgent {
    private:
        int x_pos;
        int y_pos;
        //Unknown until it's initialised into a BasicEnvironment
        vector <vector <char>> environment_tiles;
        bool is_environ_set = false;
        friend class BasicEnvironment;


    public:
        
        BasicAgent(string message, int x_bound=100, int y_bound=100) {
            x_pos = rand() % x_bound + 1;
            y_pos = rand() % y_bound + 1;
            cout << "BasicAgent created at (" << x_pos << ", " << y_pos << ")" << endl;
            cout << "BasicAgent says '" << message << "'!" << endl;
        }

        void move(char direction, int move_amount) {

            //TODO: when setting in BasicEnvironment, not updating this properly, so not able to move in current instantiation (try pointers?)
            if (!is_environ_set) {
                cout << "Agent can't move if environment is unknown..." << endl;
                return;
            }
            if (direction == 'X') {
                x_pos += move_amount;
                if (x_pos > X_BOUND) {
                    x_pos = X_BOUND;
                }
            }
            else if (direction == 'Y') {
                y_pos += move_amount;
                if (y_pos > Y_BOUND) {
                    y_pos = Y_BOUND;
                }
            }
            else {
                cout << "Unknown direction '" << direction << "', must be either 'X' or 'Y'..." << endl;
                return;
            }

            state_position(direction, move_amount);
        }

        void state_position(char dimension, int move_size) {
            cout << "BasicAgent moved " << move_size << " units on " << dimension << 
            "-axis to new position (" << x_pos << ", " << y_pos << ")" << endl;
        }
};


class BasicEnvironment {
    private:
        list <Obstacle> obstacles;
        list <Energy> energy_sources;
        list <BasicAgent> agents;
        vector <vector <char>> environment_tiles;
        int x_bound;
        int y_bound;
        int time_instance = 0;

        void insert_obstacles_energy(int num_obstacles, int num_energy_sources) {
            // Creates 'num_obstacles' objects at random positions within the environment bounds, and inserts them into the 2D board
            for (int i=0; i<num_obstacles; i++) {
                int x_pos = rand() % x_bound + 1;
                int y_pos = rand() % y_bound + 1;
                Obstacle obst(x_pos, y_pos);
                obstacles.push_back(obst);
                environment_tiles[y_pos-1][x_pos-1] = 'O';
            }

            // Creates 'num_energy_sources' objects at random positions within the environment bounds, and inserts them into the 2D board
            for (int i=0; i<num_energy_sources; i++) {
                int x_pos = rand() % x_bound + 1;
                int y_pos = rand() % y_bound + 1;
                int energy_val = rand() % MAX_ENERGY_VAL + 1;
                Energy ener(x_pos, y_pos, energy_val);
                energy_sources.push_back(ener);
                environment_tiles[y_pos-1][x_pos-1] = 'E';
            }
        }

    public:
        BasicEnvironment(int x_bound, int y_bound, int num_obstacles, int num_energy_sources) 
                        :x_bound(x_bound), y_bound(y_bound) {

            // Initalises all environment tiles as empty spaces ("-") based on supplied dimensions, 
            // before obstacles or energy sources are added
            for (int i=0; i<y_bound; i++) {
                vector <char> environment_tiles_row;
                for (int j=0; j<x_bound; j++) {
                    environment_tiles_row.push_back('-');
                }
                environment_tiles.push_back(environment_tiles_row);
            }
            insert_obstacles_energy(num_obstacles, num_energy_sources);
        }

        void state_objects() {
            for (Obstacle obst: obstacles) {
                cout << "Obstacle at (" << obst.x_pos << ", " << obst.y_pos << ")" << endl;
            }
            for (Energy ener: energy_sources) {
                cout << "Energy source at (" << ener.x_pos << ", " << ener.y_pos << ") with energy value " << ener.energy_val << endl;
            }
        }

        void environment_visualise(){
            cout << endl << endl << "Time: " << time_instance << endl;
            for (int i=0; i<environment_tiles.size(); i++) {
                for (int j=0; j<environment_tiles[i].size(); j++) {
                    cout << " " << environment_tiles[i][j] << " ";
                }
                cout << endl;
            }
        }

        //TODO: change to using pointer to BasicAgent instead?
        void insert_agent(BasicAgent agent) {
            agents.push_back(agent);
            environment_tiles[agent.y_pos - 1][agent.x_pos - 1] = 'A';

            // Updates each agent's knowledge of environment when a new agent is inserted into one
            for (BasicAgent agnt: agents) {
                agnt.environment_tiles = environment_tiles;
                agnt.is_environ_set = true;
            }
        }

        void update_agent_position() {}

};





int main() {
    srand(time(NULL));

    BasicAgent agent_1("elo", X_BOUND, Y_BOUND);
    BasicAgent agent_2("ahoy-hoy", X_BOUND, Y_BOUND);

    BasicEnvironment environment(X_BOUND, Y_BOUND, 2, 3);
    environment.state_objects();

    environment.insert_agent(agent_1);
    environment.insert_agent(agent_2);
    environment.environment_visualise();

    agent_1.move('X', 3);
    agent_2.move('Y', 5);

}