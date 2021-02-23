import os
import time
import json
import argparse


def generate_experiment_set_pairs(starting_int: int, ending_int: int = None):
    with open("analysis/experiment_settings.json") as f:
        experiment_settings = json.load(f)

    experiment_set_pairs = []
    for experiment_set in experiment_settings[starting_int:ending_int]:
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


def create_dir(choice: str, current_dir: str = None):

    choices = ("experiment_set", "experiment")
    if choice not in choices:
        raise ValueError(f"Parameter 'choice' provided to 'create_dir()' must be one of '{choices}'")

    if choice == "experiment_set":
        # Given we are running a experiment set, 
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
    else:
        output_dir = None
        dir_val = 1
        while not output_dir:
            dir_name = f"{current_dir}/experiment_{dir_val}"
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


def main(args):
    # Gets the experiment set pairs, where each in the list constitutes a complete experiment set as a list, 
    # and each in this list constitutes an experiment (which includes all the attributes and values by which 
    # to modify the default settings) 
    experiment_set_pairs = generate_experiment_set_pairs(args.starting_idx, args.ending_idx)

    # Compiles the c++ source code
    print("Compiling simulation...")
    os.system("g++ -g src/*.cpp -o /home/dan/Agent_Survival_Simulation/Agent_Survival_Simulation/src/main")
    print("Compilation complete!")


    for i, experiment_pairs in enumerate(experiment_set_pairs):

        print(f"------ Experiment set {i+1}/{len(experiment_set_pairs)} ------")

        # Sets up the default model settings, which are modified in turn by the variations within 'experiment_settings.json'
        # for each of the experiments
        default_settings = {"time_delay": 0, "time_steps": 50, "x_bound": 15, "y_bound": 15, "min_energy_to_replicate": 400, 
                            "max_energy_to_replicate": 500, "max_num_lookaheads": 1, "num_agents": 1, "num_predators": 0, 
                            "num_obstacles": 20, "num_energies": 20, "agent_choice": 3, "seek_energy": "true", 
                            "output_csv": "true", "output_dir": "outputs", "disable_cout": "true"}

        exper_set_dir = create_dir("experiment_set")

        for j, experiment in enumerate(experiment_pairs):

            # Creates the subdirectory of outputs in which to store this experiment set and experiment repetitions, 
            # and sets this to be the output directory for the experiments
            exper_dir = create_dir("experiment", exper_set_dir)
            default_settings["output_dir"] = exper_dir

            attribs = list(experiment.keys())
            attrib_vals = list(experiment.values())

            # Runs each simulation with the necessary variations made to the simulation configuration
            set_config_file(attribs, attrib_vals, default_settings)

            for k in range(args.repetitions):
                print(f"Experiment {j+1}/{len(experiment_pairs)} Repetition {k+1}/{args.repetitions}: "
                      f"Running simulation with '{attribs}' set to '{attrib_vals}'")
                os.system("src/main")
                # By sleeping 1 second, it ensures we have a unique file name for each output, 
                # as if multiple complete on same second, they overwrite each other
                time.sleep(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--starting_idx",  type=int, default=0, 
                        help="Index of the experiment set in 'experiment_settings.json' from which to start.")
    parser.add_argument("--ending_idx", type=int, 
                        help="Index of the experiment set in 'experiment_settings.json' at which to end.")
    parser.add_argument("--repetitions", type=int, default=1, 
                        help="Number of times to run each experiment within the experiment set.")
    args = parser.parse_args()

    main(args)
