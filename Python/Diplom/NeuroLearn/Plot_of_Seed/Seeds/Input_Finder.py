import tkinter as tk
from tkinter import ttk
import matplotlib.pyplot as plt
import os
import math


def update_progressbar(progressbar, value):
    progressbar["value"] = value


def main():
    num_files = 2
    selection_duration = 5000
    step = 150

    total_files_in_folder = 0
    folder_name = f"proc_4"

    # Read pol sig from pol_sig.txt file
    pol_sig_start = []
    pol_sig_end = []

    pol_sig_file_path = os.path.join(folder_name, "pol_sig.txt")

    with open(pol_sig_file_path, "r") as pol_sig_file:
        for line in pol_sig_file:
            values = line.split()
            if len(values) == 2:
                try:
                    pol_sig_start.append(float(values[0]))
                    pol_sig_end.append(float(values[1]))
                except ValueError:
                    # If the string cannot be converted to a float, skip it
                    continue

    selection_folder_name = f"test_4"
    if not os.path.exists(selection_folder_name):
        os.makedirs(selection_folder_name)

    # Split raw selection into pol sig
    for i in range(1, num_files + 1):
        file_path = os.path.join(folder_name, f"{i}.txt")
        if not os.path.exists(file_path):
            print(f"The file {file_path} does not exist.")
            continue

        t = []
        V = []
        with open(file_path, "r") as data_file:
            # Skip the first 11 lines
            for j in range(11):
                next(data_file)
            # Parse the data as a time sequence
            for line in data_file:
                values = line.split()
                if len(values) == 2:
                    try:
                        t_val = float(values[0])
                        v_val = float(values[1])
                        if pol_sig_start[i - 1] <= t_val <= pol_sig_end[i - 1]:
                            V.append(v_val)
                        else:
                            continue
                    except ValueError:
                        # If the string cannot be converted to a float, skip it
                        continue

        # Split pol sig into selection
        for j in range(1, math.ceil((len(V) - selection_duration + step) / step)):
            total_files_in_folder += 1
            selection_V = []
            current_min = j * step - step
            current_max = current_min + selection_duration
            for k in range(current_min, current_max):
                selection_V.append(V[k])

            # Creating selection files
            selection_file_path = os.path.join(selection_folder_name, f"{total_files_in_folder}.txt")
            with open(selection_file_path, "w") as selection_file:
                for num in selection_V:
                    selection_file.write(str(num) + "\n")


if __name__ == "__main__":
    main()
