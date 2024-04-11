# ClassFuzzGen

Harness Function Generator for classes
Supports libFuzzer

## How to use

### Build and run

For building project from source you need to have zig installed:
```
zig build
```

Zig is a single binary, but if you do not want it, then look up [build.zig](build.zig), it's pretty readable.

Compiled executable is at `./zig-out/bin` (can be run then by `zig build run -- <flags to program>`). For example:
```
zig build run -- -H examples/time.hpp -c Time > fuzzer.cpp
```

All examples are in [examples](examples/README.md) subfolder.

### Compiling `fuzzer.cpp`

`fuzzer.cpp` is a class fuzzer specifically crafted for your chosen class. It chains different methods in attempt to find vulnerability. For compiling `fuzzer.cpp` use same flags as you would use on common libfuzzer target (don't forget to link class cpp files). For example:
```
clang++ fuzzer.cpp examples/time.cpp -g -std=c++17 -fsanitize=fuzzer,address -o fuzzer
```

### Options

```
Options:
  -h,--help                   Print this help message and exit
  -H,--header TEXT:FILE REQUIRED
                              Header path to the class
  -c,--class TEXT REQUIRED    Name of the target class
  -t,--template TEXT:FILE [template.j2] 
                              Path to the jinja2 template
  -a,--compile-args TEXT      Compile args to clang
  -l,--call-max-length UINT [10] 
                              Maximum call chain length
```


## Source code structure

[examples](examples) - folder for example classes\
[include](include) - external dependencies\
[src](src) - fuzzer generator source code

