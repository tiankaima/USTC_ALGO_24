#include <iostream>

int find_index_no_more_than(int* array, int left, int right, int target)
{
    // find the last index i such that array[i] <= target
    // array is sorted in ascending order
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (array[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left - 1;
}

int test_main()
{
    int array[] = { 1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9 };
    int target = 5;
    int left = 0;
    int right = 10;
    int result = find_index_no_more_than(array, left, right, target);
    std::cout << result << std::endl;
}

void quick_sort(int* array, int left, int right)
{
    if (left >= right) {
        return;
    }

    int pivot = array[left];
    int i = left;
    int j = right;

    while (i < j) {
        while (i < j && array[j] >= pivot) {
            j--;
        }
        array[i] = array[j];
        while (i < j && array[i] <= pivot) {
            i++;
        }
        array[j] = array[i];
    }

    array[i] = pivot;

    quick_sort(array, left, i - 1);
    quick_sort(array, i + 1, right);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int m, R;
    std::cin >> m >> R;

    int* Xs = new int[m];

    for (int i = 0; i < m; i++) {
        std::cin >> Xs[i];
    }

    quick_sort(Xs, 0, m - 1);

    int k = 0;
    int sum = 0;

    while (k < m) {
        //        while (j < m && Xs[j] - Xs[k] <= R) {
        //            j++;
        //        }
        int j = find_index_no_more_than(Xs, k, m, Xs[k] + R);
        //        while (k < m && Xs[k] - Xs[j - 1] <= R) {
        //            k++;
        //        }
        k = find_index_no_more_than(Xs, j, m, Xs[j] + R) + 1;
        sum++;
    }

    std::cout << sum << std::endl;

    return 0;
}