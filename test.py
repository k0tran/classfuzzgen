import os
import subprocess
import sys

def run_co_test(first, second, n1, n2, expected):
    proc = subprocess.run(
        ['./ccedit_test', '-x', first, '-o h','crossover', '-x', second, '-1', str(n1), '-2', str(n2)],
        capture_output=True,
        text=True,
        check=False
    )

    return proc.stdout.strip() == expected

def test_class(hppfile, cppfile, class_name, clen, tests):
    # Render ccedit.j2
    print('Rendering class')
    os.system(f'zig build run -- -H {hppfile} -c {class_name} -t ccedit.j2 -l {clen} > ccedit_test.cpp')

    # Compile rendered cpp
    print('Compiling')
    os.system(f'clang++ ccedit_test.cpp {cppfile} -g -std=c++17 -Iinclude -fsanitize=address,leak -o ccedit_test')

    # Run crossover tests
    print('Testing crossover')
    for i, t in enumerate(tests['crossover']):
        if run_co_test(*t):
            print('Error on test: ', t)
            sys.exit(1)
        else:
            print(f'Test #{i} completed')
    print('')

TIME_TESTS = {
    'crossover': [
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 0, 'ff2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 1, 0, 'ff2c2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 2, 0, 'ff2c2c2c2c2cc6c5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 3, 0, 'ff2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 1, 'ff2c2c2c2cc5'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 2, 'ff2c2c2c2cc5'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 1, 1, 'ff2c2c2c2c2cc5'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 2, 1, 'ff2c2c2c2c2cc6c5'],
    ]
}

WALKER_TESTS = {
    'crossover': [
        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 0, 'fe2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 1, 0, 'fe2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 2, 0, 'fe2c2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 3, 0, 'fe2c2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 4, 0, 'fe2c2c2c2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 5, 0, 'fe2e2e2e2e'],

        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 1, 'fe2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 2, 'fe2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 3, 'fe2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 4, 'fe2e2e2e2e'],

        ['fe2c2c2c2c', 'fe2e2e2e2e', 1, 2, 'fe2c2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 2, 1, 'fe2c2c2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 3, 1, 'fe2c2c2c2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 3, 2, 'fe2c2c2c2e2e'],
    ]
}

if __name__ == '__main__':
    print('Time tests')
    test_class(
        'examples/time.hpp',
        'examples/time.cpp',
        'Time',
        100,
        TIME_TESTS
    )

    print('Walker tests')
    test_class(
        'examples/walker.hpp',
        'examples/walker.cpp',
        'Walker',
        100,
        WALKER_TESTS
    )


