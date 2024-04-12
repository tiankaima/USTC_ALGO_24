#include <iostream>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int m, n;
    std::cin >> m >> n;

    int* map = new int[m * n];
#define MAP(i, j) map[(i) * m + (j)]

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> MAP(i, j);
        }
    }

    int* dp = new int[m * n];
#define DP(i, j) dp[(i) * m + (j)]

    int ans = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || j == 0) {
                DP(i, j) = 1 - MAP(i, j);
            } else if (MAP(i, j) == 1) {
                DP(i, j) = 0;
            } else {
                DP(i, j) = std::min(DP(i - 1, j), std::min(DP(i, j - 1), DP(i - 1, j - 1))) + 1;
            }
            ans += DP(i, j);
        }
    }

    std::cout << ans << std::endl;

    delete[] map;
    delete[] dp;
    return 0;
}