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
