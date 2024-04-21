#include <iostream>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int M, N;
    std::cin >> N >> M;

    int* connection_u = new int[M];
    int* connection_v = new int[M];

    for (int i = 0; i < M; i++) {
        std::cin >> connection_u[i] >> connection_v[i];
        connection_u[i]--;
        connection_v[i]--;
    }

    bool* visited = new bool[N];
    int* stack = new int[N];
    int stack_top = 0;

    auto dfs1 = [&](int u) mutable -> void {
        auto ds1_impl = [&](int u, auto& ds1_impl) mutable -> void { // NOLINT(*-no-recursion)
            visited[u] = true;
            for (int i = 0; i < M; i++) {
                if (connection_u[i] == u) {
                    int v = connection_v[i];
                    if (!visited[v]) {
                        ds1_impl(v, ds1_impl);
                    }
                }
            }
            stack[stack_top++] = u;
        };
        return ds1_impl(u, ds1_impl);
    };

    int* color = new int[N];
    std::fill(color, color + N, 0);
    int scc_count = 0;

    auto dfs2 = [&](int u) mutable -> void {
        auto ds2_impl = [&](int u, auto& ds2_impl) mutable -> void { // NOLINT(*-no-recursion)
            color[u] = scc_count;
            for (int i = 0; i < M; i++) {
                if (connection_v[i] == u) {
                    int v = connection_u[i];
                    if (color[v] == 0) {
                        ds2_impl(v, ds2_impl);
                    }
                }
            }
        };
        return ds2_impl(u, ds2_impl);
    };

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            dfs1(i);
        }
    }
    for (int i = N - 1; i >= 0; i--) {
        int u = stack[i];
        if (color[u] == 0) {
            scc_count++;
            dfs2(u);
        }
    }

    std::cout << scc_count << std::endl;

    // print color as in group:
    int* printed = new int[N];
    std::fill(printed, printed + N, 0);
    for (int i = 0; i < N; i++) {
        if (printed[color[i]] == 0) {
            printed[color[i]] = 1;
            for (int j = 0; j < N; j++) {
                if (color[j] == color[i]) {
                    std::cout << j + 1 << ' ';
                }
            }
            std::cout << std::endl;
        }
    }

    delete[] connection_u;
    delete[] connection_v;
    delete[] visited;
    delete[] color;
    delete[] stack;
    return 0;
}