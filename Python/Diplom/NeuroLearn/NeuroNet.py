import random
import numpy as np
import matplotlib.pyplot as plt
import os
import tkinter as tk
from tkinter import ttk


def relu(t):
    return np.maximum(t, 0)


def softmax(t):
    out = np.exp(t)
    return out / np.sum(out)


def softmax_batch(t):
    out = np.exp(t)
    return out / np.sum(out, axis=1, keepdims=True)


def sparse_cross_entropy(z, y):
    return -np.log(z[0, y])


def sparse_cross_entropy_batch(z, y):
    return -np.log(np.array([z[j, y[j]] for j in range(len(y))]))


def predict(x):
    t1 = x @ W1 + b1
    h1 = relu(t1)
    t2 = h1 @ W2 + b2
    z = softmax(t2)
    return z


def to_full(y, num_classes):
    y_full = np.zeros((1, num_classes))
    y_full[0, y] = 1
    return y_full


def to_full_batch(y, num_classes):
    y_full = np.zeros((len(y), num_classes))
    for j, yj in enumerate(y):
        y_full[j, yj] = 1
    return y_full


def relu_dervi(t):
    return (t >= 0).astype(float)


def calc_accuracy():
    correct = 0
    for x, y in dataset:
        z = predict(x)
        y_pred = np.argmax(z)
        if y_pred == y:
            correct += 1
    acc = correct / len(dataset)
    return acc


def category_converter(val):
    if val == 0:
        pass
    elif val == 10:
        val = 1
    elif val == 2:
        val = 2
    elif val == 4:
        val = 3
    elif val == 6:
        val = 4
    elif val == 8:
        val = 5

    return val


def update_progressbar(progress_bar, value):
    progress_bar["value"] = value


# ----------------Main------------------
INPUT_DIM = 5000
OUT_DIM = 6
H_DIM = 15

x = np.random.randn(1, INPUT_DIM)
y = random.randint(0, OUT_DIM - 1)

print("Loading training sample...")
base_dir = os.path.abspath('./Plot_of_Seed/Seeds')
retrieved_files = 150 + 1
dataset = []

for directory in os.listdir(base_dir):
    full_path = os.path.join(base_dir, directory)
    if not os.path.isdir(full_path) or not directory.startswith('Selection_proc_'):
        continue
    print(directory)

    category = int(directory[len('Selection_proc_'):])
    category = int(category / 2)
    # category = category_converter(category)

    for i in range(retrieved_files, 7000):
        if not os.path.exists(f"{full_path}/{i}.txt"):
            continue

        with open(os.path.join(full_path, f"{i}.txt")) as f:
            data = [float(line.strip()) for line in f]

        # Add the data to the dataset, with the category label
        dataset.append((np.array([data]), category))

        print(f"{full_path}/{i}.txt")

print("Done")
# print(dataset)

# ---------------------------------
# ------------Layers---------------
# ---------------------------------
W1 = np.random.randn(INPUT_DIM, H_DIM)
b1 = np.random.randn(1, H_DIM)

W2 = np.random.randn(H_DIM, OUT_DIM)
b2 = np.random.randn(1, OUT_DIM)

W1 = (W1 - 0.5) * 2 * np.sqrt(1 / INPUT_DIM)
b1 = (b1 - 0.5) * 2 * np.sqrt(1 / INPUT_DIM)

W2 = (W2 - 0.5) * 2 * np.sqrt(1 / H_DIM)
b2 = (b2 - 0.5) * 2 * np.sqrt(1 / H_DIM)
# ---------------------------------

ALPHA = 0.0002
NUM_EPOCHS = 2500
BATCH_SIZE = 50

loss_arr = []

progress = 0

root = tk.Tk()
root.title("Selection creating")

progressbar = ttk.Progressbar(root, length=300, mode="determinate")
progressbar.pack()

for ep in range(NUM_EPOCHS):
    random.shuffle(dataset)
    for i in range(len(dataset) // BATCH_SIZE):
        batch_x, batch_y = zip(*dataset[i * BATCH_SIZE: i * BATCH_SIZE + BATCH_SIZE])
        x = np.concatenate(batch_x, axis=0)
        y = np.array(batch_y)
        # x, y = dataset[i]

        # Forward
        t1 = x @ W1 + b1
        h1 = relu(t1)
        t2 = h1 @ W2 + b2
        z = softmax_batch(t2)
        E = np.sum(sparse_cross_entropy_batch(z, y))

        # Backward
        y_full = to_full_batch(y, OUT_DIM)
        dE_dt2 = z - y_full
        dE_dW2 = h1.T @ dE_dt2
        dE_db2 = np.sum(dE_dt2, axis=0, keepdims=True)
        dE_dh1 = dE_dt2 @ W2.T
        dE_dt1 = dE_dh1 * relu_dervi(t1)
        dE_dW1 = x.T @ dE_dt1
        dE_db1 = np.sum(dE_dt1, axis=0, keepdims=True)

        # Update
        W1 = W1 - ALPHA * dE_dW1
        b1 = b1 - ALPHA * dE_db1
        W2 = W2 - ALPHA * dE_dW2
        b2 = b2 - ALPHA * dE_db2

        loss_arr.append(E)

    # Progress
    progress = ep / NUM_EPOCHS * 100
    print("{:.2f}".format(progress), "%")

    update_progressbar(progressbar, progress)
    root.update()
root.destroy()

accuracy = calc_accuracy()
print("W1: ", W1)
print("b1: ", b1)
print("W2: ", W2)
print("b2: ", b2)

real_accuracy = []
# Input
for directory in os.listdir(base_dir):
    acc = 0
    full_path = os.path.join(base_dir, directory)
    if not os.path.isdir(full_path) or not directory.startswith('Selection_proc_'):
        continue
    print(directory)

    category = int(directory[len('Selection_proc_'):])
    category = int(category / 2)

    for i in range(1, retrieved_files):
        if not os.path.exists(f"{full_path}/{i}.txt"):
            print(f"{full_path}/{i}.txt not exist")
            continue

        with open(os.path.join(full_path, f"{i}.txt")) as f:
            data = [float(line.strip()) for line in f]

        print(f"{full_path}/{i}.txt")

        # Add the data to the dataset, with the category label
        x = np.array([data])

        # Using calibrated NN
        class_names = ['0%', '2%', '4%', '6%', '8%', '10%']
        probs = predict(x)
        pred_class = np.argmax(probs)
        print('Predicted class:', class_names[pred_class])
        if category == pred_class:
            acc += 1

    real_accuracy.append(acc / (retrieved_files - 1) * 100)

print()
print("Training accuracy: " + "{:.2f}".format(accuracy * 100), '%')

print("Real accuracy")
print(f"0%: " + "{:.2f}".format(real_accuracy[0]), '%')
print(f"2%: " + "{:.2f}".format(real_accuracy[1]), '%')
print(f"4%: " + "{:.2f}".format(real_accuracy[2]), '%')
print(f"6%: " + "{:.2f}".format(real_accuracy[3]), '%')
print(f"8%: " + "{:.2f}".format(real_accuracy[4]), '%')
print(f"10%: " + "{:.2f}".format(real_accuracy[5]), '%')

plt.plot(loss_arr)
plt.show()
