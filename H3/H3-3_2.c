#include "stdio.h"

int n;
int data[1 * 100000000 + 1];
int k;

int main()
{
    scanf("%d %d", &n, &k);

    for (int i = 0; i < n; i++) {
        static int tmp;
        scanf("%d", &tmp);
        data[tmp]++;
    }

    for (int index = 1 * 100000000; index >= 0; index--) {
        if (data[index] > 0) {
            k -= data[index];
        }

        if (k <= 0) {
            printf("%d\n", index);
            break;
        }
    }

    return 0;
}