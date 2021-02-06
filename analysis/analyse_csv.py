import os
import sys
import re
import numpy as np
import argparse
import pandas as pd

parser = argparse.ArgumentParser()
parser.add_argument("--file_name",  type=str, help="Name of the simulation output file in 'outputs' to analyse (including extension). "
                    "If not provided, need to provide file name via input statements.")

args = parser.parse_args()



def load_df():

    # Gets the name of the file either through the optionally-provided argument or via 'input' statements and checks for validity
    file_name = None
    if args.file_name:
        fn = f"../outputs/{args.file_name}"
        if os.path.exists(fn):
            file_name = fn
        else:
            print(f"Provided '--file_name' argument '{args.file_name}' doesn't match file in 'outputs'; try again...")
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
            diffs = [int(positions[n]) - int(positions[n-1]) for n in range(1, len(positions))]
            analysis_dict[f"Agent {i+1} average d{dim}/dT"] = np.mean(diffs)

    return analysis_dict


if __name__ == "__main__":
    upper_df, lower_df = load_df()
    analysis_dict = compute_stats(upper_df, lower_df)
    print(analysis_dict)
