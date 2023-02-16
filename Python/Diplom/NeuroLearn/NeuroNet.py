import random
import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
import os


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


# ----------------Main------------------
INPUT_DIM = 4
OUT_DIM = 3
H_DIM = 15

print("Loading training sample...")
# iris = datasets.load_iris()
# dataset = [(iris.data[i][None, ...], iris.target[i]) for i in range(len(iris.target))]
data_dir = "/path/to/data/directory"
data_files = os.listdir(data_dir)
dataset = []
for data_file in data_files:
    data = np.loadtxt(os.path.join(data_dir, data_file), delimiter=",")
    for i in range(len(data)):
        x = data[i, :-1][None, ...]
        y = int(data[i, -1])
        dataset.append((x, y))
print("Done")

x = np.random.randn(1, INPUT_DIM)
y = random.randint(0, OUT_DIM - 1)

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

    print("Progress: ", round(ep / NUM_EPOCHS * 100, 2), '%')

accuracy = calc_accuracy()
print("W1: ", W1)
print("b1: ", b1)
print("W2: ", W2)
print("b2: ", b2)
print()
print("Accuracy: ", accuracy, '%')

# Input
x = np.array([7.9, 3.1, 7.5, 1.8])

# Using calibrated NN
class_names = ['Setosa', 'Versicolor', 'Virginica']
probs = predict(x)
pred_class = np.argmax(probs)
print('Predicted class:', class_names[pred_class])

plt.plot(loss_arr)
plt.show()