#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#define MAX_N 3000
#define MAX_M 6000
int n, m;

struct Edge {
    int v, w;
    //    int u, v, w;
};
std::vector<Edge> e[MAX_N];
long long dis[MAX_N], cnt[MAX_N], vis[MAX_N];

bool spfa(int s)
{
    //    std::fill(dis, dis + n, 1e9);
    //    std::fill(cnt, cnt + n, 0);
    //    std::fill(vis, vis + n, 0);
    for (int i = 0; i < n; i++) {
        dis[i] = 1e9;
        cnt[i] = 0;
        vis[i] = 0;
    }

    dis[s] = 0, vis[s] = 1;

    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop(), vis[u] = 0;
        for (auto ed : e[u]) {
            //            if (e[i].u != u) {
            //                continue;
            //            }
            int v = ed.v, w = ed.w;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) {
                    return false;
                }
                if (!vis[v]) {
                    q.push(v);
                    vis[v] = 1;
                }
            }
        }
    }
    return true;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // one direction graph, weight might be negative, duplicate e might exist
    // a negative cycle might exist, print -1 and exit if so

    std::cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        //        e[i] = { u - 1, v - 1, w };
        e[u - 1].push_back({ v - 1, w });
    }

    long long ans[MAX_N];

    for (int i = 0; i < n; i++) {
        if (!spfa(i)) {
            std::cout << -1 << std::endl;
            return 0;
        }
        ans[i] = 0;
        for (int j = 0; j < n; j++) {
            ans[i] += dis[j] * (j + 1);
            //            std::cout << dis[j] << " ";
        }
        //        std::cout << std::endl;
    }

    for (int i = 0; i < n; i++) {
        std::cout << ans[i] << std::endl;
    }
}