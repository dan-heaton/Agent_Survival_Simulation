## 0.1.12 - 27/01/21
- Fixed bug w/ BasicAgent's 'check_path()', which would result in causing the agent to leap over an obstacle when approaching 
it in a negative direction
- Further improvements to AdvancedAgent's 'seek_energy()' to introduce element of random movement if stuck in a loop

## 0.1.11 - 26/01/21
- Improvements to AdvancedAgent's 'seek_energy()' function to (seemingly) no longer 'leap' over obstacles, but now gets stuck very 
easily (need to implement either random behaviour in this case to break out, or implement lookahead; WIP)

## 0.1.10 - 25/01/21
- Separated 'seek_energy()' of BasicAgent into several subroutines, which should make constructing AdvancedAgent's 
redefined 'seek_energy()' easier to implement
- Implemented behaviour in AdvancedAgent to move around obstacles during 'seek_energy()' rather than 'leaping' over them (WIP though, 
as can get stuck in a loop)

## 0.1.9 - 23/01/21
- Improved 'tasks.json' to include all .cpp files automatically, and made 'BasicAgent::state_position()' private (as already used privately)
- Setup 'AdvancedAgent' as inheriting from 'BasicAgent', in separate .cpp/.h and using necessary polymorphism alongside own 
implementation of 'seek_energy()' (implementation of full 'seek_energy()' still WIP)

## 0.1.8 - 22/01/21
- Moved header files to separate 'include' subdirectory + modify source files to utilise these, along with change in settings
- Reorganised 'Simulation.cpp'/'Simulation.h' as a single class w/ functions now as methods, and split 'run_simulation()' into two methods
- Modified .csv's to being output to separate 'outputs' directory + w/ suffixes on names based on current datetime

## 0.1.7 - 21/01/21
- Moved running of simulation to separate .cpp/.h file
- Implemented 'timed' simulation, so that window updates with each time step (e.g. once per second)
- Fix for issue with agent picking up energy on negative x/y movement but not detecting until a positive x/y movement
- Fix for issue with instantion of obstacles/energies on top of each other, resulting in fewer obstacles/energies than desired
- Added generation of .csv from simulation

## 0.1.6 - 20/01/21
- Added 'seek_energy()' function to BasicAgent

## 0.1.5 - 19/01/21
- Added 'move_random()' method to BasicAgent, which utilises existing 'move()' method with randomised arguments
- Added 'run_simulation()' to 'main.cpp' to initiate and manage a simulation
- Minor bug fixes

## 0.1.4 - 18/01/21
- Added behaviour to BasicAgent to stop at obstacle or consume energy while moving
- Added 'name' attribute for BasicAgent in place of it's 'message'
- Implemented agent move boundary checking in both positive and negative directions, and as separate 'check_bounds()' function
- Added 'update()' method to BasicEnvironment to update its copy of the board based on agents' movements and/or consumption of energies

## 0.1.3 - 17/01/21
- Implemented pointer to BasicAgent within BasicEnvironment to enable sharing of information
- Split BasicAgent and BasicEnvironment from 'basic_simulation.cpp' w/ associated header + cpp files, and renamed 'basic_simulation.cpp' to 'main.cpp'

## 0.1.2 - 16/01/21
- Added initialisation of environment tiles, along with addition of energy sources and obstacles
- Added visualisation function of environment tiles
- Added ability to add agents to environment
- Ensured agents stay within bounds when moving
- Convert BasicAgent 'move_x()' and 'move_y()' into single 'move()' method
- (WIP) Enable BasicAgent move behaviour to reflect in environment and in own state
- Moved stuff to 'src' subdirectory

## 0.1.1 - 15/01/21
- Added to-do list
- Changed repo name and local project name from 'CPP-Project' to 'Agent_Survival_Simulation'
- Added BasicAgent and BasicEnvironment classes to 'basic_simulation.cpp'
- Added Obstacle and Energy structs to 'basic_simulation.cpp'

### 0.1.0 - 13/01/21
- Initial commit
- Added project description