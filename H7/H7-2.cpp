#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, M;
    std::cin >> n >> M;
    vector<vector<int>> connections(M, vector<int>(3));
    for (int i = 0; i < M; i++) {
        std::cin >> connections[i][0] >> connections[i][1] >> connections[i][2];
    }

    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    sort(connections.begin(), connections.end(), [](auto& a, auto& b) { return a[2] < b[2]; });
    int ans = 0;
    function<int(int)> find = [&](int x) -> int {
        if (p[x] != x) {
            p[x] = find(p[x]);
        }
        return p[x];
    };
    for (auto& e : connections) {
        int x = e[0] - 1, y = e[1] - 1, cost = e[2];
        if (find(x) == find(y)) {
            continue;
        }
        p[find(x)] = find(y);
        ans += cost;
        if (--n == 1) {
            std::cout << ans;
            return 0;
        }
    }
    return 0;
}