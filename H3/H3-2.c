#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"

#include "stdio.h"
#include "math.h"

int xs[200000 + 1];
int ys[200000 + 1];
int n;

double ans = 1e9;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX3(a, b, c) MAX(MAX(a, b), c)
#define MIN3(a, b, c) MIN(MIN(a, b), c)


void update_ans(int i, int j) {
    double distance = sqrt((xs[i] - xs[j]) * (xs[i] - xs[j]) + (ys[i] - ys[j]) * (ys[i] - ys[j]));
    ans = MIN(ans, distance);
}

void update_ans_with(int x1, int y1, int x2, int y2) {
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    ans = MIN(ans, distance);
}

void swap(int i, int j) {
    int tmp = xs[i];
    xs[i] = xs[j];
    xs[j] = tmp;

    tmp = ys[i];
    ys[i] = ys[j];
    ys[j] = tmp;
}

void quicksort_by_x(int left, int right) { // NOLINT(*-no-recursion)
    if (right - left <= 1) {
        return;
    }

    int pivot = xs[(left + right) / 2];
    int i = left;
    int j = right - 1;

    while (i <= j) {
        while (xs[i] < pivot) {
            i++;
        }

        while (xs[j] > pivot) {
            j--;
        }

        if (i <= j) {
            swap(i, j);

            i++;
            j--;
        }
    }

    quicksort_by_x(left, j + 1);
    quicksort_by_x(i, right);
}

void quicksort_by_y(int left, int right) { // NOLINT(*-no-recursion)
    if (right - left <= 1) {
        return;
    }

    int pivot = ys[(left + right) / 2];
    int i = left;
    int j = right - 1;

    while (i <= j) {
        while (ys[i] < pivot) {
            i++;
        }

        while (ys[j] > pivot) {
            j--;
        }

        if (i <= j) {
            swap(i, j);

            i++;
            j--;
        }
    }

    quicksort_by_y(left, j + 1);
    quicksort_by_y(i, right);
}

void insert_sort_by_y(int left, int right) {
    for (int i = left + 1; i < right; i++) {
        int tmp_x = xs[i];
        int tmp_y = ys[i];
        int j = i - 1;
        while (j >= left && ys[j] > tmp_y) {
            xs[j + 1] = xs[j];
            ys[j + 1] = ys[j];
            j--;
        }
        xs[j + 1] = tmp_x;
        ys[j + 1] = tmp_y;
    }
}

void merge_by_y(int left, int mid, int right) {
    int i = left;
    int j = mid;
    int k = 0;

    static int buffer_xs[200000 + 1];
    static int buffer_ys[200000 + 1];

    while (i < mid && j < right) {
        if (ys[i] < ys[j]) {
            buffer_xs[k] = xs[i];
            buffer_ys[k] = ys[i];
            i++;
        } else {
            buffer_xs[k] = xs[j];
            buffer_ys[k] = ys[j];
            j++;
        }
        k++;
    }

    while (i < mid) {
        buffer_xs[k] = xs[i];
        buffer_ys[k] = ys[i];
        i++;
        k++;
    }

    while (j < right) {
        buffer_xs[k] = xs[j];
        buffer_ys[k] = ys[j];
        j++;
        k++;
    }

    for (int i = 0; i < k; i++) {
        xs[left + i] = buffer_xs[i];
        ys[left + i] = buffer_ys[i];
    }
}

void min_distance(int left, int right) { // NOLINT(*-no-recursion)
    if (right - left <= 3) {
        for (int i = left; i < right; i++) {
            for (int j = i + 1; j < right; j++) {
                update_ans(i, j);
            }
        }
//        quicksort_by_y(left, right);
//        insert_sort_by_y(left, right);
        return;
    }

    int mid = (left + right) / 2;

    min_distance(left, mid);
    min_distance(mid, right);
//    merge_by_y(left, mid, right);

    static int buffer_xs[200000 + 1];
    static int buffer_ys[200000 + 1];

    int buffer_index = 0;

    for (int i = left; i < right; i++) {
        if (xs[i] < xs[mid] - ans || xs[i] > xs[mid] + ans) {
            continue;
        }

        for (int j = buffer_index - 1; j >= 0
//        && ys[i] - buffer_ys[j] < ans
                ; j--) {
//            update_ans(i, j);
            update_ans_with(xs[i], ys[i], buffer_xs[j], buffer_ys[j]);
        }

        buffer_xs[buffer_index] = xs[i];
        buffer_ys[buffer_index] = ys[i];
        buffer_index++;
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%u", xs + i);
        scanf("%u", ys + i);
    }

    quicksort_by_x(0, n);

//    for (int i = 0; i < n; i++) {
//        printf("%u %u\n", xs[i], ys[i]);
//    }

    min_distance(0, n);
    printf("%.4f\n", ans);
    return 0;
}

#pragma clang diagnostic pop