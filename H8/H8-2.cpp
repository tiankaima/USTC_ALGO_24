#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#define MAX_N 30000
#define MAX_M 100000
#define INF 0x3f3f3f3f
int n, m, s, t;

struct Edge {
    int v, w;
};
std::vector<Edge> e[MAX_N];
int dis[MAX_N], cnt[MAX_N], vis[MAX_N];

bool spfa()
{
    std::fill(dis, dis + n, INF);
    std::fill(cnt, cnt + n, 0);
    std::fill(vis, vis + n, 0);

    dis[s] = 0, vis[s] = 1;

    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop(), vis[u] = 0;
        for (auto ed : e[u]) {
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

    std::cin >> n >> m >> s >> t;
    s--, t--;

    struct FullEdge {
        int x, y, w, a;
    };

    std::vector<FullEdge> full_edges;
    int max_alpha = 0;
    for (int i = 0; i < m; i++) {
        int x, y, w, a;
        std::cin >> x >> y >> w >> a;
        full_edges.push_back({ x - 1, y - 1, w, a });
        max_alpha = std::max(max_alpha, a);
    }

    int min_price = INF;

    for (int i = 0; i <= max_alpha; i++) {
        for (int j = 0; j < m; j++) {
            if (full_edges[j].a == i) {
                e[full_edges[j].x].push_back({ full_edges[j].y, full_edges[j].w });
                e[full_edges[j].y].push_back({ full_edges[j].x, full_edges[j].w });
            }
        }
        //        if (!spfa())
        //            throw std::runtime_error("Negative cycle detected");
        spfa();
        if (dis[t] == INF)
            continue;
        int cur_price = dis[t] + i * 100;
        min_price = std::min(min_price, cur_price);
    }

    if (min_price == INF)
        std::cout << "-1" << std::endl;
    else
        std::cout << min_price << std::endl;
}