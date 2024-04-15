#include "walker.hpp"

#include <csignal>

Walker::Walker() : x(0), y(0) {}

void Walker::up() { y += 1; }
void Walker::left() { x -= 1; }
void Walker::down() { y -= 1; }
void Walker::right() { x += 1; }

char Walker::get() const {
    return '0' + ((x + y) % 10);
}

void Walker::set(char c) {
    if (x > 8 && c == '0')
        raise(SIGSEGV);
}


