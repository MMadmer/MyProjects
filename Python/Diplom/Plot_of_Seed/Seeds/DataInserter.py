import os

dataset = []
for directory in os.listdir('.'):
    if not os.path.isdir(directory) or not directory.startswith('Selection_proc_'):
        continue

    category = directory[len('Selection_proc_'):]

    for i in range(1, 7000):
        if not os.path.exists(f"{directory}/{i}.txt"):
            continue

        with open(os.path.join(directory, f"{i}.txt")) as f:
            data = [float(line.strip()) for line in f]

        # Add the data to the dataset, with the category label
        dataset.append((data, category))

        print(f"{directory}/{i}.txt")

print(dataset)
