#include <algorithm>
#include <stdio.h>

#define N 1005
using namespace std;

struct Node {
    int u, v;
    int val;
    bool operator<(const Node& rhs) const
    {
        return val < rhs.val;
    }
} edge[N];
int n, m, cnt, tmp;
int father[N];
int Find(int x)
{
    if (father[x] == x)
        return x;
    return father[x] = Find(father[x]);
}
void Kruskal(bool print)
{
    sort(edge, edge + cnt);

    int pos = -1;
    int res = 0, mst = 0;
    for (int i = 0; i < cnt; i++) {
        int u = edge[i].u, v = edge[i].v;
        int x = Find(u), y = Find(v);
        if (x != y) {
            father[x] = y;
            res++;
            mst += edge[i].val;
        } else {
            pos = i;
            continue;
        }
    }
    if (pos != -1)
        edge[pos] = edge[--cnt];

    if (print)
        printf("%d\n", mst);
}
int main()
{
    scanf("%d%d%d", &tmp, &n, &m);

    cnt = 0;
    for (int i = 0; i < m + n; i++) {
        scanf("%d%d%d", &edge[cnt].u, &edge[cnt].v, &edge[cnt].val);
        cnt++;
        for (int i = 0; i < N; i++)
            father[i] = i;
        Kruskal(i >= n);
    }
    return 0;
}