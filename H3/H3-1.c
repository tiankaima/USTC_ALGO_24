#include <stdio.h>

int data[2 * 100000 + 1];
int n;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) MAX(MAX(a, b), c)

int max_subarray_sum(int left, int right) // NOLINT(*-no-recursion)
{
    if (right - left <= 1) {
        return data[left];
    }

    int mid = (left + right) / 2;
    int left_sum = max_subarray_sum(left, mid);
    int right_sum = max_subarray_sum(mid, right);

    // discuss the subarray that crosses the middle point
    int cur_left_sum = 0;
    int left_max_sum = 0;

    for (int i = mid - 1; i >= left; i--) {
        cur_left_sum += data[i];
        left_max_sum = MAX(left_max_sum, cur_left_sum);
    }

    int cur_right_sum = 0;
    int right_max_sum = 0;

    for (int i = mid; i < right; i++) {
        cur_right_sum += data[i];
        right_max_sum = MAX(right_max_sum, cur_right_sum);
    }

    return MAX3(left_sum, right_sum, left_max_sum + right_max_sum);
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", data + i);
    }

    printf("%d", max_subarray_sum(0, n));

    return 0;
}