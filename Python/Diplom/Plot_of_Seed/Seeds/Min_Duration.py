import tkinter as tk
from tkinter import ttk
import matplotlib.pyplot as plt
import os
import math


def update_progressbar(progress_bar, value):
    progress_bar["value"] = value


num_files = 20
folders = range(0, 12, 2)
selection_folder_prefix = "Selection_proc"
progress = 0

root = tk.Tk()
root.title("Selection creating")

progressbar = ttk.Progressbar(root, length=300, mode="determinate")
progressbar.pack()

for folder in folders:
    folder_name = f"proc_{folder}"
    if not os.path.exists(folder_name):
        print(f"The folder {folder_name} does not exist.")
        continue

    # Read pol sig from pol_sig.txt file
    pol_sig_start = []
    pol_sig_end = []

    pol_sig_file_path = os.path.join(folder_name, "pol_sig.txt")
    if not os.path.exists(pol_sig_file_path):
        print(f"The file {pol_sig_file_path} does not exist.")
        continue

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

    selection_folder_name = f"{selection_folder_prefix}_{folder}"
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
        for j in range(1, math.ceil(len(V) / 2500)):
            selection_V = []
            current_max = j * 2500
            for k in range((j - 1) * 2500, current_max):
                selection_V.append(V[k])

            # Creating selection files
            selection_file_path = os.path.join(selection_folder_name, f"{i}_{j}.txt")
            with open(selection_file_path, "w") as selection_file:
                for num in selection_V:
                    selection_file.write(str(num) + "\n")

            fig, ax = plt.subplots(figsize=(19.2, 12), dpi=100)
            ax.plot(selection_V)
            ax.set_ylabel("V")
            # ax.set_xlabel("t")
            fig.subplots_adjust(left=0.02, bottom=0.02, right=1.0, top=1.0)
            # plt.show()
            plt.savefig(f"Selection_Seeds_Images/proc_{folder}/{i}_{j}.png", dpi=100)
            plt.close()

        # Progress
        progress += 1 / (num_files * len(folders)) * 100
        print("{:.2f}".format(progress), "%")

        update_progressbar(progressbar, progress)
        root.update()

root.destroy()
