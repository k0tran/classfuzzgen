# List of examples

## Time

Files: [time.hpp](time.hpp), [time.cpp](time.cpp)\
Vulnerability: SIGSEGV at `Time::set(23)`

## TemperatureConverter

Files: [tempconv.hpp](tempconv.hpp)\
Vulnerability: None, class with no constructor and static methods only. Fuzzer should not fuzz anything

## Walker

Files: [walker.hpp](walker.hpp), [walker.cpp](walker.cpp)\
Vulnerability: SIGSEGV wich can be achieved with 8 calls to `Walker::right()` and then `Walker::set('0')`

## Stats

Files: [stats.hpp](stats.cpp), [stats.cpp](stats.cpp)\
Vulnerability: SIGSEGV wich can be achieved if `Stats::get` goes out of bounds which can be achieved with certain combinations of `Stats::move` and `Stats::inc`
Comment: for now it's not working bc libfuzzer overuses memory even when destructor called explicitly
Poc for reaching SIGSEGV:
```cpp
Stats s(0, 0, 0);

for (size_t i = 0; i < Stats::SIZE; ++i) {
    for (size_t j = 0; j < i * 2; ++j)
        s.inc();
    
    s.move(0, 1);
    s.move(1, 1);
    s.move(2, 1);
}

s.get();
```