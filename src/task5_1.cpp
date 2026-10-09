#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool valid_date(const std::string& s) {
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;
    for (std::size_t i = 0; i < s.size(); ++i)
        if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    const int year = std::stoi(s.substr(0, 4));
    const int month = std::stoi(s.substr(5, 2));
    const int day = std::stoi(s.substr(8, 2));
    if (year < 1 || month < 1 || month > 12) return false;
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool leap = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    if (leap) days[1] = 29;
    return day >= 1 && day <= days[month - 1];
}

struct Event {
    std::string date;
    int delta;
};

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "hotel.txt";
    std::ifstream input(path);
    if (!input) { std::cerr << "Cannot open input file\n"; return 1; }
    std::size_t n, rooms;
    if (!(input >> n >> rooms)) return 1;
    std::vector<Event> events;
    events.reserve(2 * n);
    for (std::size_t i = 0; i < n; ++i) {
        std::string arrival, departure;
        if (!(input >> arrival >> departure) || !valid_date(arrival) ||
            !valid_date(departure) || departure < arrival) {
            std::cerr << "Invalid reservation on line " << i + 2 << '\n';
            return 1;
        }
        if (arrival == departure) continue; // A zero-night stay uses no room.
        events.push_back({arrival, +1});
        events.push_back({departure, -1});
    }
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        if (a.date != b.date) return a.date < b.date;
        return a.delta < b.delta; // Departures free rooms before same-day arrivals.
    });
    std::size_t occupied = 0, maximum = 0;
    for (const Event& event : events) {
        if (event.delta == -1) --occupied;
        else maximum = std::max(maximum, ++occupied);
    }
    std::cout << (maximum <= rooms ? "YES" : "NO") << '\n';
}
