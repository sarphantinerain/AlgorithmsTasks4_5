#include <algorithm>
#include <iostream>
#include <vector>

void insertion_sort(std::vector<long long>& a, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        long long value = a[i];
        int j = i;
        while (j > left && value < a[j - 1]) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = value;
    }
}

// k is an absolute, zero-based index in the original array.
long long select_k(std::vector<long long>& a, int left, int right, int k) {
    while (true) {
        if (right - left + 1 <= 32) {
            insertion_sort(a, left, right);
            return a[k];
        }
        int medians = 0;
        for (int start = left; start <= right; start += 5) {
            int end = std::min(start + 4, right);
            insertion_sort(a, start, end);
            int middle = start + (end - start) / 2;
            std::swap(a[left + medians], a[middle]);
            ++medians;
        }
        const long long pivot = select_k(a, left, left + medians - 1,
                                         left + medians / 2);
        int less = left, current = left, greater = right;
        while (current <= greater) {
            if (a[current] < pivot) std::swap(a[less++], a[current++]);
            else if (a[current] > pivot) std::swap(a[current], a[greater--]);
            else ++current;
        }
        if (k < less) right = less - 1;
        else if (k > greater) left = greater + 1;
        else return pivot;
    }
}

int main() {
    int n, k;
    if (!(std::cin >> n >> k) || n < 1 || k < 1 || k > n) return 1;
    std::vector<long long> a(n);
    for (auto& value : a) if (!(std::cin >> value)) return 1;
    std::cout << select_k(a, 0, n - 1, k - 1) << '\n';
}
