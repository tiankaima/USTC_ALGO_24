#include <stdio.h>
#include <stdlib.h>

int* gen_shuffled_array(int n)
{
    int cur;
    int offset = 1;
    int flip = 0;
    int next_start = 0;
    int* result = (int*)malloc(n * sizeof(int));
    int result_index = 0;

    while (next_start < n) {
        if (flip) {
            cur = next_start + offset;
            // next_start = next_start;
        } else {
            cur = next_start;
            next_start += offset;
        }

        while (cur < n) {
            result[result_index++] = cur;
            cur += offset * 2;
        }

        if (cur - offset < n) {
            flip = 0;
        } else {
            flip = 1;
        }
        offset *= 2;
    }

    return result;
}

__attribute__((unused)) void test_main()
{
    int* index = gen_shuffled_array(7);
    int k[7] = { 1, 2, 3, 4, 6, 5, 7 };
    int result[7];

    for (int i = 0; i < 7; i++) {
        result[index[i]] = k[i];
    }

    for (int i = 0; i < 7; i++) {
        printf("%d ", result[i]);
    }

    free(index);
}

int main()
{
    int n;
    scanf("%d", &n);
    int* input = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &input[i]);
    }
    int* index = gen_shuffled_array(n);
    int* result = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[index[i]] = input[i];
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", result[i]);
    }

    free(input);
    free(index);
    free(result);
    return 0;
}