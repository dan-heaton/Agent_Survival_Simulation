#include <iostream>
#include<cstdlib>
#include<ctime>
#include<list>

using namespace std;

const int MAX_ENERGY_VAL = 100;
const int X_BOUND = 100;
const int Y_BOUND = 100;


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


    public:
        
        BasicAgent(string message, int x_bound=100, int y_bound=100) {
            x_pos = rand() % x_bound + 1;
            y_pos = rand() % y_bound + 1;
            cout << "BasicAgent created at x-pos " << x_pos << " and y-pos " << y_pos << "." << endl;
            cout << "BasicAgent says '" << message << "'!" << endl;
        }
        
        void move_x(int x_move) {
            x_pos += x_move;
            state_position("X", x_move);
        };

        void move_y(int y_move) {
            y_pos += y_move;
            state_position("Y", y_move);
        };

        void state_position(string dimension, int move_size) {
            cout << "BasicAgent moved " << move_size << " units on " << dimension << 
            "-axis to new x-pos " << x_pos << " and y-pos " << y_pos << "." << endl;
        }
};


class BasicEnvironment {
    private:
        list <Obstacle> obstacles;
        list <Energy> energy_sources;
        int x_bound;
        int y_bound;

    public:
        BasicEnvironment(int x_bound, int y_bound, int num_obstacles, int num_energy_sources) 
                        :x_bound(x_bound), y_bound(y_bound) {
            
            // Creates 'num_obstacles' objects at random positions within the environment bounds
            for (int i=0; i<num_obstacles; i++) {
                int x_pos = rand() % x_bound + 1;
                int y_pos = rand() % y_bound + 1;
                Obstacle obst(x_pos, y_pos);
                obstacles.push_back(obst);
            }

            // Creates 'num_energy_sources' objects at random positions within the environment bounds
            for (int i=0; i<num_energy_sources; i++) {
                int x_pos = rand() % x_bound + 1;
                int y_pos = rand() % y_bound + 1;
                int energy_val = rand() % MAX_ENERGY_VAL + 1;
                Energy ener(x_pos, y_pos, energy_val);
                energy_sources.push_back(ener);
            }
        }

        void state_objects() {
            for (Obstacle obst: obstacles) {
                cout << "Obstacle at (" << obst.x_pos << ", " << obst.y_pos << ")" << endl;
            }
            for (Energy ener: energy_sources) {
                cout << "Energy source at (" << ener.x_pos << ", " << ener.y_pos << ") with energy value " << ener.energy_val << endl;
            }
        }

        void visualise(){

        }

};





int main() {
    srand(time(NULL));

    BasicAgent agent_1("elo", X_BOUND, Y_BOUND);
    agent_1.move_x(5);
    BasicAgent agent_2("ahoy-hoy", X_BOUND, Y_BOUND);
    agent_2.move_y(10);

    BasicEnvironment environment(X_BOUND, Y_BOUND, 3, 5);
    environment.state_objects();
}