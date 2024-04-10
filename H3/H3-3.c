#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
#include "stdio.h"

int n;
int data[8 * 1000000 + 1];
int k;

//void quicksort(int left, int right) { // NOLINT(*-no-recursion)
//    if (right - left <= 1) {
//        return;
//    }
//
//    int pivot = data[(left + right) / 2];
//    int i = left;
//    int j = right - 1;
//
//    while (i <= j) {
//        while (data[i] < pivot) {
//            i++;
//        }
//
//        while (data[j] > pivot) {
//            j--;
//        }
//
//        if (i <= j) {
//            int tmp = data[i];
//            data[i] = data[j];
//            data[j] = tmp;
//
//            i++;
//            j--;
//        }
//    }
//
//    quicksort(left, j + 1);
//    quicksort(i, right);
//}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void merge_sort_with_k_max(int left, int right) {
    // this would mean:
    // 1: everything is sorted max to min,
    // 2: only the first k values have meanings, other shall be discarded
    // other process are just like regular merge sort:

    if (right <= left + 1) {
        return;
    }

    int mid = (left + right) / 2;
    merge_sort_with_k_max(left, mid);
    merge_sort_with_k_max(mid, right);

    int i = left;
    int j = mid;
    int k_ = 0;
    int tmp[right - left];

    while (i < mid && j < right && k_ < k) {
        if (data[i] > data[j]) {
            tmp[k_++] = data[i++];
        } else {
            tmp[k_++] = data[j++];
        }
    }

    while (i < mid && k_ < k) {
        tmp[k_++] = data[i++];
    }

    while (j < right && k_ < k) {
        tmp[k_++] = data[j++];
    }

    for (int i_ = 0; i_ < MIN(k_, k); i_++) {
        data[left + i_] = tmp[i_];
    }
}
#pragma clang diagnostic pop

int main() {
    scanf("%d %d", &n, &k);

    for (int i = 0; i < n; i++) {
        scanf("%d", &data[i]);
    }

//    quicksort(0, n);

    merge_sort_with_k_max(0, n);


    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    printf("%d\n", data[k - 1]);

    return 0;
}
#pragma clang diagnostic pop