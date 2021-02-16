import os
import time


def create_dir():
    # Given we are running a new batch of experiments, 
    # creates a new subdirectory within 'outputs' to hold these
    output_dir = None
    dir_val = 1
    while not output_dir:
        dir_name = f"outputs/experiment_set_{dir_val}"
        if not os.path.exists(dir_name):
            os.mkdir(dir_name)
            output_dir = dir_name
        else:
            dir_val += 1
    return output_dir


def set_config_file(modified_attr: str, modified_attr_val: str, default_settings: dict):    
    modified_settings = {**default_settings, modified_attr: modified_attr_val}
    print(f"Setting config file w/ '{modified_attr} = {modified_attr_val}'...")
    lines = [f"{k} = {v}\n" for k, v in modified_settings.items()]
    # Don't want a newline at the end of the config file
    lines[-1] = lines[-1].split("\n")[0]
    with open("config.txt", "w") as f:
        f.writelines(lines)
    f.close()


if __name__ == "__main__":

    # Compiles the c++ source code
    print("Compiling simulation...")
    os.system("g++ -g src/*.cpp -o /home/dan/Agent_Survival_Simulation/Agent_Survival_Simulation/src/main")
    print("Compilation complete!")

    # Creates the subdirectory of outputs in which to store this run of experiments
    output_dir = create_dir()


    default_settings = {"time_delay": 0, "time_steps": 50, "x_bound": 15, "y_bound": 15, "min_energy_to_replicate": 400, 
                        "max_energy_to_replicate": 500, "max_num_lookaheads": 1, "num_agents": 1, "num_predators": 0, 
                        "num_obstacles": 20, "num_energies": 20, "agent_choice": 3, "seek_energy": "true", 
                        "output_csv": "true", "output_dir": output_dir, "disable_cout": "true"}
    variations = {"num_obstacles": [0, 5, 10, 20], "num_energies": [10, 20, 40]}

    # Runs each simulation with the necessary variations made to the simulation configuration
    for attr, attr_vals in variations.items():
        for attr_val in attr_vals:
            set_config_file(attr, attr_val, default_settings)
            print(f"Running simulation with '{attr} = {attr_val}'")
            os.system("src/main")
            # By sleeping 1 second, it ensures we have a unique file name for each output, 
            # as if multiple complete on same second, they overwrite each other
            time.sleep(1)
