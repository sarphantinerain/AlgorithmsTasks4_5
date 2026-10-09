#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "patients.txt";
    std::ifstream input(path);
    if (!input) { std::cerr << "Cannot open input file\n"; return 1; }
    std::size_t n;
    if (!(input >> n) || n < 1 || n > 100'000) return 1;
    std::vector<std::int64_t> times(n);
    for (auto& time : times)
        if (!(input >> time) || time < 1 || time > 100'000) return 1;
    std::sort(times.begin(), times.end());
    std::int64_t elapsed = 0, total_wait = 0;
    for (std::int64_t time : times) {
        total_wait += elapsed; // This patient waits for all previous visits.
        elapsed += time;
    }
    std::cout << total_wait << '\n';
}
