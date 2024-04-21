#include <iostream>
#include <algorithm>

typedef struct AB {
    int A;
    int B;
} AB;

bool cmp(const AB& a, const AB& b)
{
    return (a.B == b.B) ? (a.A > b.A) : (a.B < b.B);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;

    AB* ABs = new AB[N];

    for (int i = 0; i < N; i++) {
        std::cin >> ABs[i].A >> ABs[i].B;
    }

    std::sort(ABs, ABs + N, cmp);

    int find_cur = 0;
    int cur = 1;
    int* dp = new int[30000 + 1];
    dp[0] = 0;

    while (cur <= 30000) {
        dp[cur] = dp[cur - 1];

        while (find_cur < N && ABs[find_cur].B < cur) {
            find_cur++;
        }

        if (ABs[find_cur].B == cur) {
            dp[cur] = std::max(dp[cur], dp[ABs[find_cur].A] + 1);
        }
        cur++;
    }

//    for (int i = 1; i < 30000; i++) {
//        std::cout << dp[i] << ' ';
//    }

    std::cout << dp[30000] << '\n';

    delete[] ABs;
    delete[] dp;
}