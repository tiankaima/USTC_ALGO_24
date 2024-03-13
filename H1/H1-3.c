#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    int n, x;
    scanf("%d %d", &n, &x);

    int count = 0;
    int k = 1;

    do {
        if (x == 0) {
            count += (n / (k * 10) - 1) * k;
            count += MIN(n % (k * 10) + 1, k);
        } else {
            count += (n / (k * 10)) * k;
            count += MIN(MAX(n % (k * 10) - k * x + 1, 0), k);
        }
        k *= 10;
    } while (k < n);

    printf("%d", count);
    return 0;
}