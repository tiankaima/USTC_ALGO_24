import numpy as np
import matplotlib.pyplot as plt

x = np.array([[1, 2], [2, 3], [3, 3], [2, 1], [3, 2]])
y = np.array([1, 1, 1, -1, -1])

plt.scatter(x[y == 1, 0], x[y == 1, 1], c='r', label='positive')
plt.scatter(x[y == -1, 0], x[y == -1, 1], c='b', label='negative')

w = np.array([-1, 2])
b = -2

x1 = np.linspace(0, 4, 100)
x2 = (-w[0] * x1 - b) / w[1]

plt.plot(x1, x2, label='separating hyperplane')

plt.fill_between(x1, x2 - 1 / np.linalg.norm(w), x2 + 1 / np.linalg.norm(w), alpha=0.5, label='margin boundary')

plt.legend()
plt.show()