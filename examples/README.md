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
PoC and Hex of CC is in [test_stats.cpp](test_stats.cpp)
**WARN:** Check if your fuzzer build has room for such callchain
**WARN2:** `using std::size_t;` is necessary for libclang
