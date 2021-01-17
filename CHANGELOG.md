## 0.1.3 - 16/01/21
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