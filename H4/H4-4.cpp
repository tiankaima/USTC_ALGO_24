#include <iostream>

int LIS(const int* array, int size)
{
    int* dp = new int[size];
    int max = 0;

    for (int i = 0; i < size; i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++) {
            if (array[j] < array[i]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        max = std::max(max, dp[i]);
    }

    delete[] dp;
    return max;
}

int LCS(const int* array_a, const int* array_b, int size)
{
    // call LIS:
    int* index_a = new int[size];
    for (int i = 0; i < size; i++) {
        index_a[array_a[i]] = i;
    }

    int* new_b = new int[size];
    for (int i = 0; i < size; i++) {
        new_b[i] = index_a[array_b[i]];
    }

    int result = LIS(new_b, size);
    delete[] index_a;
    delete[] new_b;
    return result;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;

    int* array_a = new int[n];
    int* array_b = new int[n];

    for (int i = 0; i < n; i++) {
        std::cin >> array_a[i];
    }

    for (int i = 0; i < n; i++) {
        std::cin >> array_b[i];
    }

    std::cout << LCS(array_a, array_b, n) << std::endl;

    delete[] array_a;
    delete[] array_b;
    return 0;
}