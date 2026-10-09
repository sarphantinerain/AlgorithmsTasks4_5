#include <cstdint>
#include <iostream>
#include <vector>

// Count cross inversions while merging [left, middle) and [middle, right).
std::int64_t count_inversions(std::vector<std::int64_t>& a,
                              std::vector<std::int64_t>& buffer,
                              std::size_t left, std::size_t right) {
    if (right - left < 2) return 0;
    const std::size_t middle = left + (right - left) / 2;
    std::int64_t count = count_inversions(a, buffer, left, middle)
                       + count_inversions(a, buffer, middle, right);
    std::size_t i = left, j = middle, out = left;
    while (i < middle && j < right) {
        if (a[i] <= a[j]) {
            buffer[out++] = a[i++];
        } else {
            // The remaining left-half values are all greater than a[j].
            count += static_cast<std::int64_t>(middle - i);
            buffer[out++] = a[j++];
        }
    }
    while (i < middle) buffer[out++] = a[i++];
    while (j < right) buffer[out++] = a[j++];
    for (std::size_t k = left; k < right; ++k) a[k] = buffer[k];
    return count;
}

int main() {
    std::size_t n;
    if (!(std::cin >> n) || n > 1'000'000) return 1;
    std::vector<std::int64_t> a(n), buffer(n);
    for (auto& value : a) if (!(std::cin >> value)) return 1;
    std::cout << count_inversions(a, buffer, 0, n) << '\n';
}
