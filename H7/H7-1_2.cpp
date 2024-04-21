#include <iostream>

int connection_u[100000 + 1];
int connection_v[100000 + 1];
bool visited[30000 + 1];
int stack[30000 + 1];
int stack_top = 0;
int color[30000 + 1];
int scc_count = 0;
int M, N;

void dfs1(int u)
{
    visited[u] = true;
    for (int i = 0; i < M; i++) {
        if (connection_u[i] == u) {
            int v = connection_v[i];
            if (!visited[v]) {
                dfs1(v);
            }
        }
    }
    stack[stack_top++] = u;
}

void dfs2(int u)
{
    color[u] = scc_count;
    for (int i = 0; i < M; i++) {
        if (connection_v[i] == u) {
            int v = connection_u[i];
            if (color[v] == 0) {
                dfs2(v);
            }
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> N >> M;

    for (int i = 0; i < M; i++) {
        std::cin >> connection_u[i] >> connection_v[i];
    }

    for (int i = 1; i <= N; i++) {
        if (!visited[i]) {
            dfs1(i);
        }
    }
    for (int i = N; i > 0; i--) {
        int u = stack[i];
        if (color[u] == 0) {
            scc_count++;
            dfs2(u);
        }
    }

    std::cout << scc_count << std::endl;
    return 0;
}