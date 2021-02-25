import os
import sys
import re
import json
import numpy as np
import argparse
import pandas as pd


def load_df(fn: str, dir_path: str = None):

    # Gets the name of the file either through the optionally-provided argument or via 'input' statements and checks for validity
    file_name = None
    if fn:
        file_path = f"{dir_path}/{fn}" if dir_path else f"outputs/{fn}"
        if os.path.exists(file_path):
            file_name = file_path
        else:
            print(f"Provided '--file_name' argument '{fn}' doesn't match file in 'outputs'; try again...")
            sys.exit()

    else:
        found_file = False
        while not found_file:
            date = input("Date of file creation (YYYY-MM-DD / YYYY-M-D): ")
            time = input("Time of file creation (HH-MM-SS, 24h): ")
            fn = f"../outputs/simulation_{date}-{time}.csv"
            if os.path.exists(fn):
                file_name = fn
                found_file = True
            else:
                print(f"File name '{fn}' not found; try again...")

    df = pd.read_csv(file_name)

    # Creates the 'upper' part (containing configuration line)
    upper_df = df.iloc[:1, :]

    # Creates the 'lower' part (containing the state of the board at specific time increments), 
    # without the extra columns needed in the configuration line
    headers = df.iloc[1].dropna()
    lower_df = pd.DataFrame(df.dropna(axis=1).values[2:], columns=list(headers))

    return upper_df, lower_df


def compute_stats(upper_df, lower_df):

    analysis_dict = {}

    num_starting_energies = upper_df.loc[0, "# Energies"]
    num_ending_energies = lower_df.loc[lower_df.index[-1], "# Energies Remaining"]
    num_energies_consumed = num_starting_energies - float(num_ending_energies)
    percent_consumed = round((num_energies_consumed/num_starting_energies)*100, 2)
    analysis_dict["Percentage energies consumed"] = percent_consumed

    num_moves = lower_df.loc[lower_df.index[-1], "Time Step"]
    analysis_dict["Average energies consumed per move"] = round(num_energies_consumed / float(num_moves), 2)
    
    num_agents = len(re.findall("X-Pos", " ".join(list(lower_df.columns))))
    for i in range(num_agents):
        for dim in ("X", "Y"):
            positions = lower_df.loc[:, f"Agent {i+1} {dim}-Pos"]
            diffs = [abs(int(positions[n]) - int(positions[n-1])) for n in range(1, len(positions))]
            analysis_dict[f"Agent {i+1} average d{dim}/dT"] = np.mean(diffs)

    return analysis_dict


def output_analysis(args_config_dict: dict, meta_analysis_dict: dict):
    
    # Selects the modifications made to default config settings for this specific experiment set 
    # and experiment via querying the 'experiment_settings.json' file used by 'run_experiments.py'
    with open("analysis/experiment_settings.json") as f:
        experiment_settings = json.load(f)
    experiment_set_dict = experiment_settings[int(args_config_dict["Experiment set"])-1]
    experiment_config_dict = {k: v[int(args_config_dict["Experiment"])-1] for k, v in experiment_set_dict.items()}


    # Combines the config settings used for the simulation and the analysis results into one output
    # (also includes an 'breaker' column between)
    output_dict = {**args_config_dict, "-+-+-+": "-+-+-+", **experiment_config_dict, 
                   "+-+-+-": "+-+-+-", **meta_analysis_dict}

    # Puts this dictionary into a DataFrame and either appends or writes this to the output file, 
    # depending on whether a file already exists there or not
    file_name = "analysis/outputs_analysis.csv"
    df = pd.DataFrame.from_dict({k: [v] for k, v in output_dict.items()})
    if os.path.exists(file_name):
        df.to_csv(file_name, mode="a", index=False, header=False)
        print(f"Appending analysis dict to '{file_name}'...")
    else:
        df.to_csv(file_name, mode="w", index=False)
        print(f"Writing analysis dict to '{file_name}'...")


def main(args):
    if args.file_name:
        upper_df, lower_df = load_df(args.file_name)
        analysis_dict = compute_stats(upper_df, lower_df)
    

    if args.directory:
        dir_path = f"outputs/{args.directory}"
        inner_dir_paths = sorted([f"{dir_path}/{fn}" for fn in os.listdir(dir_path)])
        for experiment_path in inner_dir_paths:
            analysis_dicts = []
            for file_name in sorted(os.listdir(experiment_path)):
                upper_df, lower_df = load_df(file_name, experiment_path)
                analysis_dicts.append(compute_stats(upper_df, lower_df))

            # Creates the info about what and how the experiments were run by 'run_experiments.py'
            args_config_dict = {"Experiment set": args.directory.split("_")[-1], 
                                "Experiment": experiment_path.split("_")[-1], 
                                "Num Repeptitions": len(os.listdir(experiment_path))}

            # Averages over each of those repititions of an experiment (each an individual file)
            # (also assumes that the keys for each dictionary are the same as each other, which should be the case)
            meta_analysis_dict = {k: round(np.mean([a_d[k] for a_d in analysis_dicts]), 2) for k in analysis_dicts[0].keys()}
            if args.no_console_output:
                print(meta_analysis_dict)
            if args.output_results:
                output_analysis(args_config_dict, meta_analysis_dict)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--file_name",  type=str, help="Name of the simulation output file in 'outputs' to analyse "
                        "(including extension). If not provided, need to provide file name via input statements.")
    parser.add_argument("--directory", type=str, help="Name of the directory within 'outputs' for which to analyse "
                        "all the files.")
    parser.add_argument("--output_results", action="store_true", help="Set true if wish to output the analysis "
                        "results to .csv output, along with the simulation configurations.")
    parser.add_argument("--no_console_output", action="store_false", help="Set false if wish to specifically not " 
                        "print the analyses to console output.")
    args = parser.parse_args()

    main(args)
