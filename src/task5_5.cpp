#include <algorithm>
#include <iostream>
#include <vector>

void insertion_sort(std::vector<long long>& a, std::size_t left, std::size_t right) {
    for (std::size_t i = left + 1; i < right; ++i) {
        long long value = a[i];
        std::size_t j = i;
        while (j > left && value < a[j - 1]) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = value;
    }
}

// [left, right). Exactly one recursive call per loop iteration: sort the
// smaller partition recursively, then process the larger one in the loop.
void quick_sort(std::vector<long long>& a, std::size_t left,
                std::size_t right, int depth_limit) {
    while (right - left > 24) {
        if (depth_limit == 0) {
            std::make_heap(a.begin() + left, a.begin() + right);
            std::sort_heap(a.begin() + left, a.begin() + right);
            return;
        }
        --depth_limit;
        long long x = a[left], y = a[left + (right - left) / 2], z = a[right - 1];
        if (x > y) std::swap(x, y);
        if (y > z) std::swap(y, z);
        if (x > y) std::swap(x, y);
        const long long pivot = y; // Median of three values.

        std::size_t less = left, current = left, greater = right;
        while (current < greater) {
            if (a[current] < pivot) std::swap(a[less++], a[current++]);
            else if (a[current] > pivot) std::swap(a[current], a[--greater]);
            else ++current;
        }
        std::size_t small_left, small_right;
        std::size_t large_left, large_right;
        if (less - left < right - greater) {
            small_left = left; small_right = less;
            large_left = greater; large_right = right;
        } else {
            small_left = greater; small_right = right;
            large_left = left; large_right = less;
        }
        if (small_right - small_left > 1)
            quick_sort(a, small_left, small_right, depth_limit);
        left = large_left;
        right = large_right;
    }
    insertion_sort(a, left, right);
}

int main() {
    std::size_t n;
    if (!(std::cin >> n)) return 1;
    std::vector<long long> a(n);
    for (auto& value : a) if (!(std::cin >> value)) return 1;
    int depth_limit = 0;
    for (std::size_t length = n; length > 1; length /= 2) depth_limit += 2;
    quick_sort(a, 0, n, depth_limit);
    for (std::size_t i = 0; i < n; ++i)
        std::cout << (i ? " " : "") << a[i];
    std::cout << '\n';
}
