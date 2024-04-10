#include "stdio.h"

int n;
int data[1 * 100000000 + 1];
int k;

void select_sort(int left, int right) {
    int max = data[left];
    int max_index = left;
    for (int i = left + 1; i < right; i++) {
        if (data[i] > max) {
            max = data[i];
            max_index = i;
        }
    }

    int tmp = data[left];
    data[left] = data[max_index];
    data[max_index] = tmp;
}

int main() {
    scanf("%d %d", &n, &k);

    for (int i = 0; i < n; i++) {
        static int tmp;
        scanf("%d", &tmp);
        data[i] = tmp;
    }

    for (int i = 0; i < k; i++) {
        select_sort(i, n);
    }

    printf("%d\n", data[k - 1]);
    return 0;
}