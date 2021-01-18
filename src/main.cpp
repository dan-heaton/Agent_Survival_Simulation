#include <iostream>
#include "BasicAgent.h"
#include "BasicEnvironment.h"

using namespace std;


int main() {
    srand(time(NULL));
    const int X_BOUND = 15;
    const int Y_BOUND = 15;

    BasicAgent agent_1("elo", X_BOUND, Y_BOUND);
    BasicAgent agent_2("ahoy-hoy", X_BOUND, Y_BOUND);

    BasicEnvironment environment(X_BOUND, Y_BOUND, 20, 20);
    environment.state_objects();

    environment.insert_agent(&agent_1);
    environment.insert_agent(&agent_2);
    environment.environment_visualise();

    agent_1.move('X', 15);
    agent_2.move('Y', 15);
    environment.update();
    environment.environment_visualise();

    cout << "Success!" << endl;
}