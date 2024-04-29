#include "time.hpp"

#include <csignal> // for segfault

const double DELAY = 1;

Time::Time() : t(0) { sleep(DELAY); }

Time::Time(uint ms) : t(ms) { sleep(DELAY); }

void Time::set(uint ms) {
    // intentional security hole:
    sleep(DELAY);
    if (ms == 23)
        raise(SIGSEGV);
    t = ms;
}

void Time::zero() {
    sleep(DELAY);
    t = 0;
}

uint Time::get() const {
    sleep(DELAY);
    return t;
}

float Time::secs() const {
    sleep(DELAY);
    return t / 1000.0;
}

bool Time::is_zero() const {
    sleep(DELAY);
    return t == 0;
}

