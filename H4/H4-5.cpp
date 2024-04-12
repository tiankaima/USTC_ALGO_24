#include <iostream>
#include <limits>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    int* ps = new int[n + 1];

    for (int i = 0; i < n; i++) {
        std::cin >> ps[i] >> ps[i + 1];
    }

    int* m = new int[n * n];

#define M(i, j) m[(i - 1) * n + (j - 1)]

    for (int i = 1; i <= n; i++) {
        M(i, i) = 0;
    }

    for (int l = 2; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;

            M(i, j) = std::numeric_limits<int>::max();
            for (int k = i; k < j; k++) {
                M(i, j) = std::min(M(i, j), M(i, k) + M(k + 1, j) + ps[i - 1] * ps[k] * ps[j]);
            }
        }
    }
    std::cout << M(1, n) << std::endl;

    delete[] ps;
    delete[] m;
    return 0;
}