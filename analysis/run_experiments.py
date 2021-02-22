import os
import time
import json


def generate_experiment_set_pairs(starting_int: int = 0):
    with open("analysis/experiment_settings.json") as f:
        experiment_settings = json.load(f)

    experiment_set_pairs = []
    for experiment_set in experiment_settings[starting_int:]:
        experiment_pairs = []
        # Note: assumes that, for each experiment set w/ multiple attributes being modified, 
        # we have the same number of variations for each attribute
        num_experiment_pairs = len((experiment_set[list(experiment_set.keys())[0]]))
        for i in range(num_experiment_pairs):
            experiment_settings = {}
            for attrib, variations in experiment_set.items():
                experiment_settings[attrib] = variations[i]
            experiment_pairs.append(experiment_settings)
        experiment_set_pairs.append(experiment_pairs)
    
    return experiment_set_pairs


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


def set_config_file(modified_attrs: list, modified_attr_vals: list, default_settings: dict):    
    modified_settings = {**default_settings, **{k: v for k, v in zip(modified_attrs, modified_attr_vals)}}
    lines = [f"{k} = {v}\n" for k, v in modified_settings.items()]
    # Don't want a newline at the end of the config file
    lines[-1] = lines[-1].split("\n")[0]
    with open("config.txt", "w") as f:
        f.writelines(lines)
    f.close()


if __name__ == "__main__":

    # Gets the experiment set pairs, where each in the list constitutes a complete experiment set as a list, 
    # and each in this list constitutes an experiment (which includes all the attributes and values by which 
    # to modify the default settings) 
    experiment_set_pairs = generate_experiment_set_pairs()

    # Compiles the c++ source code
    print("Compiling simulation...")
    os.system("g++ -g src/*.cpp -o /home/dan/Agent_Survival_Simulation/Agent_Survival_Simulation/src/main")
    print("Compilation complete!")


    for i, experiment_pairs in enumerate(experiment_set_pairs):

        print(f"Experiment set {i+1}/{len(experiment_set_pairs)}")

        # Creates the subdirectory of outputs in which to store this experiment set
        output_dir = create_dir()

        # Sets up the default model settings, which are modified in turn by the variations within 'experiment_settings.json'
        # for each of the experiments
        default_settings = {"time_delay": 0, "time_steps": 50, "x_bound": 15, "y_bound": 15, "min_energy_to_replicate": 400, 
                            "max_energy_to_replicate": 500, "max_num_lookaheads": 1, "num_agents": 1, "num_predators": 0, 
                            "num_obstacles": 20, "num_energies": 20, "agent_choice": 3, "seek_energy": "true", 
                            "output_csv": "true", "output_dir": output_dir, "disable_cout": "true"}

        for j, experiment in enumerate(experiment_pairs):
            attribs = list(experiment.keys())
            attrib_vals = list(experiment.values())

            # Runs each simulation with the necessary variations made to the simulation configuration
            set_config_file(attribs, attrib_vals, default_settings)
            print(f"Experiment {j+1}/{len(experiment_pairs)}: Running simulation with '{attribs}' set to '{attrib_vals}'")
            os.system("src/main")
            # By sleeping 1 second, it ensures we have a unique file name for each output, 
            # as if multiple complete on same second, they overwrite each other
            time.sleep(1)
