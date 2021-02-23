import os
import sys
import re
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
            # Averages over each of those repititions of an experiment (each an individual file)
            # (also assumes that the keys for each dictionary are the same as each other, which should be the case)
            meta_analysis_dict = {k: round(np.mean([a_d[k] for a_d in analysis_dicts]), 2) for k in analysis_dicts[0].keys()}
            print(meta_analysis_dict)
            sys.exit()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--file_name",  type=str, help="Name of the simulation output file in 'outputs' to analyse "
                        "(including extension). If not provided, need to provide file name via input statements.")
    parser.add_argument("--directory", type=str, help="Name of the directory within 'outputs' for which to analyse "
                        "all the files.")
    args = parser.parse_args()

    main(args)
