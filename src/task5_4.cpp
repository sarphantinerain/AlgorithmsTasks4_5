#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "pairs.txt";
    std::ifstream input(path);
    if (!input) { std::cerr << "Cannot open input file\n"; return 1; }
    std::size_t n;
    std::int64_t target;
    if (!(input >> n >> target) || n < 1 || n > 1'000'000 ||
        target < -100 || target > 1'000'000'000) return 1;
    std::vector<std::int64_t> a(n);
    for (auto& value : a)
        if (!(input >> value) || value < -1'000'000'000 || value > 1'000'000'000)
            return 1;
    std::sort(a.begin(), a.end());
    for (std::size_t i = 1; i < n; ++i)
        if (a[i] == a[i - 1]) { std::cerr << "Values must be distinct\n"; return 1; }

    std::size_t left = 0, right = n - 1;
    std::int64_t count = 0;
    while (left < right) {
        const std::int64_t sum = a[left] + a[right];
        if (sum == target) {
            ++count;
            ++left;
            --right;
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }
    std::cout << count << '\n';
}
