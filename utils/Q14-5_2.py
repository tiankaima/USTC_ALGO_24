import numpy as np


def resolve_pre_order(n, root, i, j):
    if i < 1 or i > n or j < 1 or j > n or i > j:
        return []
        return []
    if i == j:
        return [i]

    pre_order = []
    r = root[i - 1][j - 1]
    pre_order.append(r)
    tmp = resolve_pre_order(n, root, i, r - 1)
    pre_order.extend(tmp)
    tmp = resolve_pre_order(n, root, r + 1, j)
    pre_order.extend(tmp)
    return pre_order


def optimal_bst(n, p, q):
    e = np.zeros((n + 2, n + 1))
    w = np.zeros((n + 2, n + 1))
    root = np.zeros((n, n), dtype=int)

    for i in range(1, n + 2):
        e[i][i - 1] = q[i - 1]
        w[i][i - 1] = q[i - 1]

    for l in range(1, n + 1):
        for i in range(1, n - l + 2):
            j = i + l - 1
            e[i][j] = float("inf")
            w[i][j] = w[i][j - 1] + p[j - 1] + q[j]
            for r in range(i, j + 1):
                t = e[i][r - 1] + e[r + 1][j] + w[i][j]
                if t < e[i][j]:
                    e[i][j] = t
                    root[i - 1][j - 1] = r

    return e[1][n], resolve_pre_order(n, root, 1, n)


n = 7
p = [0.04, 0.06, 0.08, 0.02, 0.10, 0.12, 0.14]
q = [0.06, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05]

result = optimal_bst(n, p, q)
print(result)
