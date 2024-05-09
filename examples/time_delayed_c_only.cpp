#include "time.hpp"

#include <csignal> // for segfault
#include <chrono>
#include <iostream>
#include <thread>

#define DELAY 2
#define sleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

Time::Time() : t(0) { sleep(DELAY); }

Time::Time(uint ms) : t(ms) { sleep(DELAY); }

void Time::set(uint ms) {
    // intentional security hole:
    if (ms == 23)
        raise(SIGSEGV);
    t = ms;
}

void Time::zero() {
    t = 0;
}

uint Time::get() const {
    return t;
}

float Time::secs() const {
    return t / 1000.0;
}

bool Time::is_zero() const {
    return t == 0;
}

