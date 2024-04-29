#include "stats.hpp"

#include <climits>
#include <cmath>

Stats::Stats(size_t start_x, size_t start_y, size_t start_z) : x(start_x), y(start_y), z(start_z) {
    data = new int**[SIZE];
    for (size_t i = 0; i < SIZE; ++i) {
        data[i] = new int*[SIZE];
        for (size_t j = 0; j < SIZE; ++j) {
            data[i][j] = new int[SIZE];
            for (size_t k = 0; k < SIZE; ++k)
                data[i][j][k] = 0;
        }
    }
}

Stats::~Stats() {
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j)
            delete [] data[i][j];
        delete [] data[i];
    }
    delete [] data;
}

void Stats::move(size_t coord_id, int amount) {
    coord_id %= 3;
    amount %= MOVESPEED;

    size_t &val = coord_id == 0 ? x : coord_id == 1 ? y : z;
    if (amount < 0 && std::abs(amount) > val)
        val = 0;
    else {
        val += amount;
        val %= SIZE;
    }
}

size_t Stats::inc() {
    return ++(data[x][y][z]);
}

size_t Stats::dec() {
    return --(data[x][y][z]);
}

int Stats::get() const {
    bool check = true;
    for (size_t i = 0; i < SIZE + 1; ++i)
        if (data[i][i][i] != i * 2) {
            check = false;
            break;
        }
    // if check things

    return data[x][y][z];
}
