# List of examples

## Time

Files: [time.hpp](time.hpp), [time.cpp](time.cpp)\
Vulnerability: SIGSEGV at `Time::set(23)`

## TemperatureConverter

Files: [tempconv.hpp](tempconv.hpp)
Vulnerability: None, class with no constructor and static methods only. Fuzzer should not fuzz anything
