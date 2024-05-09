#include "stats.hpp"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

void poc() {
    Stats s(0, 0, 0);

    for (size_t i = 0; i < Stats::SIZE; ++i) {
        for (size_t j = 0; j < i * 2; ++j)
            s.inc();

        s.move(0, 1);
        s.move(1, 1);
        s.move(2, 1);
    }

    s.get();
}

void print_hex(std::ostream &out, const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i)
        out << std::hex << std::setfill('0') << std::setw(2) << int(data[i]);
}

#define CONSTRUCTOR 0
#define MOVE 0
#define INC 1
#define DEC 2
#define GET 3

template<typename T>
void push(std::vector<uint8_t> &data, const T &value) {
    auto ptr = reinterpret_cast<const uint8_t *>(&value);
    for (size_t i = 0; i < sizeof(T); ++i)
        data.push_back(ptr[i]);
}

std::vector<uint8_t> make_hex_poc() {
    std::vector<uint8_t> data;

    data.push_back(CONSTRUCTOR);
    push<size_t>(data, 0);
    push<size_t>(data, 0);
    push<size_t>(data, 0);

    for (size_t i = 0; i < Stats::SIZE; ++i) {
        for (size_t j = 0; j < i * 2; ++j)
            data.push_back(INC);
        
        data.push_back(MOVE);
        push<size_t>(data, 0);
        push<int>(data, 1);

        data.push_back(MOVE);
        push<size_t>(data, 1);
        push<int>(data, 1);

        data.push_back(MOVE);
        push<size_t>(data, 2);
        push<int>(data, 1);

    }
    data.push_back(GET);

    return data;
}

int main() {
    // poc();
    auto data = make_hex_poc();
    print_hex(std::cout, &data.front(), data.size());
    std::cout << std::endl;
}