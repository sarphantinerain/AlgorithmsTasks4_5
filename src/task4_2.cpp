#include <algorithm>
#include <iostream>
#include <vector>

constexpr std::size_t insertion_cutoff = 32;

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

void merge_sort(std::vector<long long>& a, std::vector<long long>& buffer,
                std::size_t left, std::size_t right) {
    if (right - left <= insertion_cutoff) {
        insertion_sort(a, left, right);
        return;
    }
    const std::size_t middle = left + (right - left) / 2;
    merge_sort(a, buffer, left, middle);
    merge_sort(a, buffer, middle, right);
    if (a[middle - 1] <= a[middle]) return; // Both halves already fit together.
    std::size_t i = left, j = middle, out = left;
    while (i < middle && j < right)
        buffer[out++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < middle) buffer[out++] = a[i++];
    while (j < right) buffer[out++] = a[j++];
    std::copy(buffer.begin() + left, buffer.begin() + right, a.begin() + left);
}

int main() {
    std::size_t n;
    if (!(std::cin >> n)) return 1;
    std::vector<long long> a(n), buffer(n); // One extra data array.
    for (auto& value : a) if (!(std::cin >> value)) return 1;
    merge_sort(a, buffer, 0, n);
    for (std::size_t i = 0; i < n; ++i)
        std::cout << (i ? " " : "") << a[i];
    std::cout << '\n';
}
