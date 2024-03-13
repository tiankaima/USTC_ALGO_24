#include <stdio.h>
#include <stdlib.h>

long long merge_sort(int *arr, int *tmp, int left, int right) { // NOLINT(*-no-recursion)
    if (left >= right) return 0;

    int mid = (left + right) / 2;
    long long inv_count = merge_sort(arr, tmp, left, mid) + merge_sort(arr, tmp, mid + 1, right);

    int i = left, j = mid + 1;

    for (int k = left; k <= right; k++)
        tmp[k] = arr[k];
    for (int k = left; k <= right; k++) {
        if (i == mid + 1)
            arr[k] = tmp[j++];
        else if (j == right + 1 || tmp[i] <= tmp[j])
            arr[k] = tmp[i++];
        else {
            arr[k] = tmp[j++];
            inv_count += mid - i + 1;
        }
    }

    return inv_count;
}

int main() {
    int n;
    scanf("%d", &n);

    int *arr = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    long long inversion = 0;

    int *tmp = (int *) malloc(n * sizeof(int));
    inversion = merge_sort(arr, tmp, 0, n - 1);

    printf("%lld", inversion);
}