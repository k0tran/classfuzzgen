#pragma once

#include <cstdlib>

class Walker {
public:
    Walker();

    void up();
    void left();
    void down();
    void right();

    char get() const;
    void set(char);

private:
    int x, y;
};
