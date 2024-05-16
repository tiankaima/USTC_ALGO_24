import numpy as np
import imageio
import sys
import networkx as nx
import random
from copy import deepcopy


class Mincut:
    def __init__(self, texture, rows, cols):
        self.texture = texture
        self.variance = np.var(texture)
        self.patch_rows, self.patch_cols, _ = texture.shape
        self.real_rows = rows
        self.real_cols = cols
        self.im_rows = rows + self.patch_rows
        self.im_cols = cols + self.patch_cols
        self.old = np.zeros((self.im_rows, self.im_cols, 3), dtype=int)
        self.new = np.zeros((self.im_rows, self.im_cols, 3), dtype=int)
        self.mask = np.zeros((self.im_rows, self.im_cols), dtype=int)
        self.overlap_zone = np.zeros((self.im_rows, self.im_cols), dtype=int)
        self.seams = np.zeros((self.im_rows, self.im_cols, 2), dtype=int)
        self.init_value_seams = np.zeros(
            (self.im_rows, self.im_cols, 2), dtype=int)
        self.max_pixel = self.patch_rows * 30
        self.min_pixel = self.patch_cols * 14
        self.border_mask = [self.im_rows, 0, self.im_rows, 0]
        self.index = 1
        self.overlap_rows = 0
        self.overlap_cols = 0

    def update_mask(self, t):
        maxi = min(t[0] + self.patch_rows, self.im_rows)
        maxj = min(t[1] + self.patch_cols, self.im_cols)
        self.mask[t[0]: maxi, t[1]: maxj] = 1
        self.border_mask[0] = min(self.border_mask[0], t[0])
        self.border_mask[1] = max(self.border_mask[1], t[0] + self.patch_rows)
        self.border_mask[2] = min(self.border_mask[2], t[1])
        self.border_mask[3] = max(self.border_mask[3], t[1] + self.patch_cols)

    def update_init_value_seams(self, corner_overlap, corner, mask_seam):
        for i in range(self.patch_rows):
            for j in range(self.patch_cols):
                x_crt = corner[0] + i
                y_crt = corner[1] + j
                if (corner_overlap[0] <= x_crt < corner_overlap[0] + self.overlap_rows and
                    corner_overlap[1] <= y_crt < corner_overlap[1] + self.overlap_cols and
                        mask_seam[x_crt - corner_overlap[0], y_crt - corner_overlap[1]] == 2):
                    self.init_value_seams[x_crt, y_crt] = [i, j]
                else:
                    self.init_value_seams[x_crt, y_crt] = [i, j]

    def init(self):
        t = [0, 0]
        self.old[t[0]: t[0] + self.patch_rows, t[1]: t[1] + self.patch_cols] = (
            self.texture
        )
        self.new = deepcopy(self.old)
        self.update_mask(t)
        self.update_init_value_seams(
            t, t, np.ones((self.patch_rows, self.patch_cols), dtype=int)
        )

    def num_neighbors_in_mask(self, p):
        return sum(
            self.mask[p[0] + i, p[1] + j] == 0
            for i in range(-1, 2)
            for j in range(-1, 2)
            if 0 <= p[0] + i < self.im_rows and 0 <= p[1] + j < self.im_cols
        )

    def num_neighbors_in_overlap(self, p):
        return sum(
            self.overlap_zone[p[0] + i, p[1] + j] == 0
            for i in range(-1, 2)
            for j in range(-1, 2)
            if 0 <= p[0] + i < self.im_rows and 0 <= p[1] + j < self.im_cols
        )

    def update_overlap_zone(self, t):
        self.overlap_zone.fill(0)
        corner = [0, 0]
        self.overlap_rows = self.overlap_cols = 0
        first = True
        n = 0

        for u in range(self.patch_rows):
            for v in range(self.patch_cols):
                if self.mask[t[0] + u, t[1] + v] == 1:
                    self.overlap_zone[t[0] + u, t[1] + v] = 1
                    if first:
                        corner = [t[0] + u, t[1] + v]
                        first = False
                    if n == 0:
                        self.overlap_rows += 1
                    n += 1
            if n and n > self.overlap_cols:
                self.overlap_cols = n
            n = 0

        for u in range(corner[0] - 1, corner[0] + self.overlap_rows):
            for v in range(corner[1] - 1, corner[1] + self.overlap_cols):
                if (
                    self.overlap_zone[u, v] == 1
                    and self.num_neighbors_in_overlap([u, v]) >= 1
                ):
                    self.overlap_zone[u, v] = (
                        2 if self.num_neighbors_in_mask([u, v]) >= 1 else 3
                    )

        return corner

    def update_seams(self, corner, mask_seam, patch_index):
        for i in range(self.overlap_rows):
            for j in range(self.overlap_cols):
                x, y = corner[0] + i, corner[1] + j
                self.seams[x, y] = [0, 1]
                mask_val = mask_seam[i, j]
                if (
                    i < mask_seam.shape[0] - 1
                    and mask_seam[i + 1, j] != mask_val
                    and mask_seam[i + 1, j] != 0
                ):
                    self.seams[x, y][1] = 2
                    if mask_val == 2:
                        self.seams[x, y][0] = patch_index
                    elif mask_val == 1 and self.seams[x, y][0] == 0:
                        self.seams[x, y][0] = 1
                if (
                    j < mask_seam.shape[1] - 1
                    and mask_seam[i, j + 1] != mask_val
                    and mask_seam[i, j + 1] != 0
                ):
                    self.seams[x, y][1] = 3 if self.seams[x, y][1] == 2 else 4
                    if mask_val == 2:
                        self.seams[x, y][0] = patch_index
                    elif mask_val == 1 and self.seams[x, y][0] == 0:
                        self.seams[x, y][0] = 1

    def entire_patch_matching_placement(self):
        patching = np.zeros((self.real_rows, self.real_cols), dtype=float)
        tk = np.zeros(
            (self.real_cols, self.patch_rows, self.patch_cols, 3), dtype=float
        )
        msk = np.zeros((self.real_cols, self.patch_rows,
                       self.patch_cols), dtype=int)
        si = np.zeros((self.real_rows, self.real_cols))

        for i in range(self.real_rows):
            w1 = min(self.patch_rows, self.real_rows - i)
            for j in range(self.real_cols):
                w2 = min(self.patch_cols, self.real_cols - j)
                msk[j].fill(0)
                a1 = self.texture[:w1, :w2]
                a2 = self.old[i: i + w1, j: j + w2]
                tk[j, :w1, :w2] = a1 - a2
                msk[j, :w1, :w2] = self.mask[i: i + w1, j: j + w2]
                si[i, j] = w1 * w2
            for k in range(3):
                patching[i] += np.sum((tk[..., k] * msk) ** 2, axis=(1, 2))

        patching /= si
        patching = np.exp(-patching / (0.3 * self.variance)).reshape(-1)
        patching /= patching.sum()
        l = np.random.choice(self.real_rows * self.real_cols, p=patching)
        return [l // self.real_cols, l % self.real_cols]

    def compute_cost_edge(self, x_crt, y_crt, x_adj, y_adj, A, B):
        new_crt, old_crt = B[x_crt, y_crt], A[x_crt, y_crt]
        new_adj, old_adj = B[x_adj, y_adj], A[x_adj, y_adj]
        r = abs(old_crt[0] - new_crt[0]) + abs(old_adj[0] - new_adj[0])
        g = abs(old_crt[1] - new_crt[1]) + abs(old_adj[1] - new_adj[1])
        b = abs(old_crt[2] - new_crt[2]) + abs(old_adj[2] - new_adj[2])
        return r + g + b

    def graph_cut_patch_placement(self, overlap_corner, corner):
        self.update_init_value_seams(overlap_corner, corner, self.overlap_zone)
        graph = nx.DiGraph()
        source, sink = "s", "t"

        for i in range(self.patch_rows - 1):
            for j in range(self.patch_cols - 1):
                x_crt, y_crt = corner[0] + i, corner[1] + j
                if self.mask[x_crt, y_crt] == 1 and self.mask[x_crt + 1, y_crt] == 1:
                    graph.add_edge(
                        f"{x_crt},{y_crt}",
                        f"{x_crt + 1},{y_crt}",
                        weight=self.compute_cost_edge(
                            x_crt, y_crt, x_crt + 1, y_crt, self.old, self.texture
                        ),
                    )
                if self.mask[x_crt, y_crt] == 1 and self.mask[x_crt, y_crt + 1] == 1:
                    graph.add_edge(
                        f"{x_crt},{y_crt}",
                        f"{x_crt},{y_crt + 1}",
                        weight=self.compute_cost_edge(
                            x_crt, y_crt, x_crt, y_crt + 1, self.old, self.texture
                        ),
                    )
                if (
                    self.mask[x_crt + 1, y_crt] == 1
                    and self.mask[x_crt + 1, y_crt + 1] == 1
                ):
                    graph.add_edge(
                        f"{x_crt + 1},{y_crt}",
                        f"{x_crt + 1},{y_crt + 1}",
                        weight=self.compute_cost_edge(
                            x_crt + 1,
                            y_crt,
                            x_crt + 1,
                            y_crt + 1,
                            self.old,
                            self.texture,
                        ),
                    )
                if (
                    self.mask[x_crt, y_crt + 1] == 1
                    and self.mask[x_crt + 1, y_crt + 1] == 1
                ):
                    graph.add_edge(
                        f"{x_crt},{y_crt + 1}",
                        f"{x_crt + 1},{y_crt + 1}",
                        weight=self.compute_cost_edge(
                            x_crt,
                            y_crt + 1,
                            x_crt + 1,
                            y_crt + 1,
                            self.old,
                            self.texture,
                        ),
                    )

        for i in range(self.patch_rows):
            for j in range(self.patch_cols):
                x_crt, y_crt = corner[0] + i, corner[1] + j
                graph.add_edge(
                    source,
                    f"{x_crt},{y_crt}",
                    weight=0 if self.overlap_zone[x_crt,
                                                  y_crt] != 2 else sys.maxsize,
                )
                graph.add_edge(
                    f"{x_crt},{y_crt}",
                    sink,
                    weight=0 if self.overlap_zone[x_crt,
                                                  y_crt] != 3 else sys.maxsize,
                )

        cut_value, partition = nx.minimum_cut(graph, source, sink)
        reachable, non_reachable = partition

        mask_seam = np.ones((self.patch_rows, self.patch_cols), dtype=int)
        for i in range(self.patch_rows):
            for j in range(self.patch_cols):
                x_crt, y_crt = corner[0] + i, corner[1] + j
                if f"{x_crt},{y_crt}" in reachable:
                    mask_seam[i, j] = 2

        self.update_seams(overlap_corner, mask_seam, self.index)

    def compute(self):
        self.init()
        num_patches = 0

        while num_patches < 300:
            corner = self.entire_patch_matching_placement()
            overlap_corner = self.update_overlap_zone(corner)
            self.graph_cut_patch_placement(overlap_corner, corner)

            for i in range(self.patch_rows):
                for j in range(self.patch_cols):
                    x, y = corner[0] + i, corner[1] + j
                    if self.seams[x, y][0] == self.index:
                        self.new[x, y] = self.texture[i, j]

            self.old = deepcopy(self.new)
            self.update_mask(corner)
            self.index += 1
            num_patches += 1

            if self.index % 10 == 0:
                print(f"Index: {self.index}")

        return self.new[: self.real_rows, : self.real_cols]


def main(texture_image_path, output_image_path, rows, cols):
    texture = imageio.imread(texture_image_path)
    if len(texture.shape) == 2:
        texture = np.stack((texture,) * 3, axis=-1)

    mincut = Mincut(texture, rows, cols)
    result = mincut.compute()
    imageio.imwrite(output_image_path, result.astype(np.uint8))
    print(f"Synthesized texture saved to {output_image_path}")


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print(
            "Usage: python script.py <texture_image_path> <output_image_path> <rows> <cols>"
        )
        sys.exit(1)

    texture_image_path = sys.argv[1]
    output_image_path = sys.argv[2]
    rows = int(sys.argv[3])
    cols = int(sys.argv[4])

    main(texture_image_path, output_image_path, rows, cols)
