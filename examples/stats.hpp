#pragma once

#include <csignal>

class Stats {
public:
    static const size_t SIZE = 1000;
    static const size_t MOVESPEED = 5;

    Stats(size_t, size_t, size_t);

    void move(size_t, int);
    
    size_t inc();
    
    size_t dec();
    
    int get() const;

    ~Stats();

private:
    int ***data;

    size_t x, y, z;
};
