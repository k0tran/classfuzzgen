import os
import subprocess
import sys

def run_test(cchex, args):
    cli_args = ['./ccedit_test', '-x', cchex, '-oh'] + args
    proc = subprocess.run(
        cli_args,
        capture_output=True,
        text=True,
        check=False
    )
    return proc.stdout.strip()

def run_tests(test_group_name, test):
    print('Testing', test_group_name)
    for i, t in enumerate(test):
        args = TEST_ARG_MAPPERS[test_group_name](*t[1:-1])
        res = run_test(t[0], args)
        if res != t[-1]:
            print('Error on test: ', t)
            print('Expected:', t[-1], 'Got:', res)
            sys.exit(1)
        else:
            print(f'Test #{i} completed')
    print('')

def test_class(hppfile, cppfile, class_name, clen, tests):
    # Render ccedit.j2
    print('Rendering class')
    os.system(f'zig build run -- -H {hppfile} -c {class_name} -t ccedit.j2 -l {clen} > ccedit_test.cpp')

    # Compile rendered cpp
    print('Compiling')
    os.system(f'clang++ ccedit_test.cpp {cppfile} -g -std=c++17 -Iinclude -fsanitize=address,leak -o ccedit_test')

    for test_group_name in tests.keys():
        run_tests(test_group_name, tests[test_group_name])

def co_test_map(second_cc_hex, n1, n2):
    return ['crossover', '-x', second_cc_hex, '-1', str(n1), '-2', str(n2)]

def rm_test_map(n):
    return ['rm', '-n', str(n)]

def ins_test_map(n, c):
    return ['ins', '-n', str(n), '-c', str(c)]

def seta_test_map(n, k, arg_hex):
    return ['seta', '-n', str(n), '-k', str(k), '-x', arg_hex]

def set_test_map(n, c):
    return ['set', '-n', str(n), '-c', str(c)]

TEST_ARG_MAPPERS = {
    'crossover': co_test_map,
    'rm': rm_test_map,
    'ins': ins_test_map,
    'seta': seta_test_map,
    'set': set_test_map,
}

TIME_TESTS = {
    'crossover': [
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 0, 'ff2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 1, 0, 'ff2c2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 2, 0, 'ff2c2c2c2c2cc6c5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 3, 0, 'ff2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 1, 'ff2c2c2c2cc4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 0, 2, 'ff2c2c2c2cc5c4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 1, 1, 'ff2c2c2c2c2cc4'],
        ['ff2c2c2c2c2cc6', 'ffa5a5a5a5c5c4', 2, 1, 'ff2c2c2c2c2cc6c4'],
    ],
    'rm': [
        ['fe2cc6c0', 0, 'fec6c0'],
        ['fe2cc6c0', 1, 'fe2cc0'],
        ['fe2cc6c0', 2, 'fe2cc6'],
        ['fe2cc6c0', 3, 'fec6c0'],

        ['fe00aaaaaaaa0100bbbbbbbb02', 0, 'fe0100bbbbbbbb02'],
        ['fe00aaaaaaaa0100bbbbbbbb02', 1, 'fe00aaaaaaaa00bbbbbbbb02'],
        ['fe00aaaaaaaa0100bbbbbbbb02', 2, 'fe00aaaaaaaa0102'],
        ['fe00aaaaaaaa0100bbbbbbbb02', 3, 'fe00aaaaaaaa0100bbbbbbbb'],
        ['fe00aaaaaaaa0100bbbbbbbb02', 0, 'fe0100bbbbbbbb02'],
    ],
    'ins': [
        ['fe', 0, 0, 'fe0000000000'],
        ['fe', 0, 1, 'fe01'],
        ['fe', 0, 2, 'fe02'],
        ['fe', 0, 3, 'fe03'],
        ['fe', 0, 4, 'fe04'],

        ['fe010203', 0, 4, 'fe04010203'],
        ['fe010203', 1, 4, 'fe01040203'],
        ['fe010203', 2, 4, 'fe01020403'],
        ['fe010203', 3, 4, 'fe01020304'],
        ['fe010203', 4, 4, 'fe04010203'],
    ],
    'seta': [
        ['ff2c2c2c2c2cc6c3bbbbbbbb2c', -1, -1, 'aaaaaaaa', 'ffaaaaaaaa2cc6c3bbbbbbbb2c'],
        ['ff2c2c2c2c2cc6c3bbbbbbbb2c', -1, 0, 'aaaaaaaa', 'ffaaaaaaaa2cc6c3bbbbbbbb2c'],
        
        ['ff2c2c2c2c2cc6c3bbbbbbbb2c', 2, -1, 'aaaaaaaa', 'ff2c2c2c2c2cc6c3aaaaaaaa2c'],
        ['ff2c2c2c2c2cc6c3bbbbbbbb2c', 2, 0, 'aaaaaaaa', 'ff2c2c2c2c2cc6c3aaaaaaaa2c'],
    ],
    'set': [
        ['ffaaaaaaaa2c2e', -1, 0, '002c2e'],
        ['ffaaaaaaaa2c2e', -1, 1, '01000000002c2e'],

        ['ffaaaaaaaa2c2e', 0, 1, 'ffaaaaaaaa012e'],
        ['ffaaaaaaaa2c2e', 0, 0, 'ffaaaaaaaa00000000002e'],
        ['ffaaaaaaaa2c2e', 1, 0, 'ffaaaaaaaa2c0000000000'],
        ['ffaaaaaaaa2c2e', 2, 0, 'ffaaaaaaaa00000000002e'],

        ['0004000300000000006883be05010102', 1, 2, '00040200006883be'],
    ],
}

WALKER_TESTS = {
    'crossover': [
        ['fe2c2c2c2c', 'fe2e2e2e2e', 0, 0, 'fe2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 1, 0, 'fe2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 2, 0, 'fe2c2c2e2e2e2e'],
        ['fe2c2c2c2c', 'fe2e2e2e2e', 3, 0, 'fe2c2c2c2e2e2e2e'],
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
    ],
    'rm': [
        ['fe2c2c2e2e', 0, 'fe2c2e2e'],
        ['fe2c2c2e2e', 1, 'fe2c2e2e'],
        ['fe2c2c2e2e', 2, 'fe2c2c2e'],
        ['fe2c2c2e2e', 3, 'fe2c2c2e'],
    ],
    'ins': [
        ['fe2c2c2e2e', 0, 0, 'fe002c2c2e2e'],
        ['fe2c2c2e2e', 1, 0, 'fe2c002c2e2e'],
        ['fe2c2c2e2e', 2, 0, 'fe2c2c002e2e'],
        ['fe2c2c2e2e', 3, 0, 'fe2c2c2e002e'],
        ['fe2c2c2e2e', 4, 0, 'fe2c2c2e2e00'],
        ['fe2c2c2e2e', 5, 0, 'fe002c2c2e2e'],
    ],
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


