#include "stdbool.h"
#include "stdio.h"

int bh(int* values, bool* reds, int i, int j, int depth) // NOLINT(*-no-recursion)
{
    //    if (i >= j)
    //        return -1;

    if (j == i + 1) {
        return (reds[i]) ? 1 : 2;
    }

    int k = i + 1;
    while (k < j && values[k] < values[i])
        k++;

    int left_bh, right_bh;
    bool left_red, right_red;

    if (i + 1 < k) {
        left_bh = bh(values, reds, i + 1, k, depth + 1);
        left_red = reds[i + 1];
        if (left_bh == -1) {
            return -1;
        }
    } else {
        left_bh = 1;
        left_red = false;
    }

    if (k < j) {
        right_bh = bh(values, reds, k, j, depth + 1);
        right_red = reds[k];
        if (right_bh == -1) {
            return -1;
        }
    } else {
        right_bh = 1;
        right_red = false;
    }

    if (left_bh != right_bh || (reds[i] && (left_red || right_red))) {
        return -1;
    }

    return left_bh + ((reds[i]) ? 0 : 1);
}

int main()
{
    int n, size, tmp;
    scanf("%d", &n);

    int values[20000 + 1];
    bool reds[20000 + 1];

    for (int i = 0; i < n; i++) {
        scanf("%d", &size);

        for (int j = 0; j < size; j++) {
            scanf("%d", &tmp);
            values[j] = (tmp > 0) ? tmp : -tmp;
            reds[j] = (tmp > 0) ? false : true;
        }

        if (reds[0] || bh(values, reds, 0, size, 0) == -1) {
            printf("N\n");
        } else {
            printf("Y\n");
        }
    }
    return 0;
}