#include <iostream>
#include <queue>

#define INF 0x3f3f3f3f
int n, m, s, t;
int ans, dis[520010];
int count = 1, now[520010], head[520010];

struct node {
    int to, net;
    int val;
} e[520010];

void add(int u, int v, int w)
{
    e[++count].to = v;
    e[count].val = w;
    e[count].net = head[u];
    head[u] = count;

    e[++count].to = u;
    e[count].val = 0;
    e[count].net = head[v];
    head[v] = count;
}

int bfs()
{
    for (int i = 1; i <= n; i++)
        dis[i] = INF;
    std::queue<int> q;
    q.push(s);
    dis[s] = 0;
    now[s] = head[s];
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int i = head[x]; i; i = e[i].net) {
            int v = e[i].to;
            if (e[i].val > 0 && dis[v] == INF) {
                q.push(v);
                now[v] = head[v];
                dis[v] = dis[x] + 1;
                if (v == t)
                    return 1;
            }
        }
    }
    return 0;
}

int dfs(int x, int sum)
{
    if (x == t)
        return sum;
    int k, res = 0;
    for (int i = now[x]; i && sum; i = e[i].net) {
        now[x] = i;
        int v = e[i].to;
        if (e[i].val > 0 && (dis[v] == dis[x] + 1)) {
            k = dfs(v, std::min(sum, e[i].val));
            if (k == 0)
                dis[v] = INF;
            e[i].val -= k;
            e[i ^ 1].val += k;
            res += k;
            sum -= k;
        }
    }
    return res;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> n >> m >> s >> t;
    int u, v, w;
    for (int i = 1; i <= m; i++) {
        std::cin >> u >> v >> w;
        add(u, v, w);
    }
    while (bfs()) {
        ans += dfs(s, INF);
    }
    std::cout << ans << std::endl;
    return 0;
}