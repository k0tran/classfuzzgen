import subprocess
import re
import sys

RUNS=10

def run(filename, max_time=60, target_error_fn='Time::set'):
    proc = subprocess.run(["/bin/time", "--format=elapsed: %e", filename, "-max_total_time=" + str(max_time),\
    "-dump_coverage=1"], capture_output=True, text=True, check=False)

    output = proc.stderr

    if re.findall(r"ERROR", output) and target_error_fn not in output:
        print("Found an error\nCaptured output:\nSTART\n" + output + "END")
    
    cov = int(re.findall(r"cov: \d+", output)[-1].split(' ')[-1])

    elapsed = float(re.findall(r"elapsed: .+", output)[-1].split(' ')[-1])

    return (cov, elapsed)


def main():
    if len(sys.argv) < 2:
        return

    cov = []
    elapsed = []
    print("Starting benchmark")
    for i in range(RUNS):
        c, e = run(sys.argv[1])
        print('Completed #' + str(i + 1), 'out of', RUNS, f'[c: {c}, e: {e}]')
        cov.append(c)
        elapsed.append(e)
    
    print('All done')
    print('Avg coverage:', sum(cov) / len(cov))
    print('Max cov:', max(cov))
    print('Min cov:', min(cov))
    cov = sorted(cov)
    if RUNS % 2 != 0:
        print('Median cov:', cov[RUNS // 2])
    else:
        print('Median cov:', (cov[RUNS // 2] + cov[(RUNS // 2) + 1]) / 2)

    print('Avg elapsed:', sum(elapsed) / len(elapsed))
    print('Max elapsed:', max(elapsed))
    print('Min elapsed:', min(elapsed))
    elapsed = sorted(elapsed)
    if RUNS % 2 != 0:
        print('Median elapsed:', elapsed[RUNS // 2])
    else:
        print('Median elapsed:', (elapsed[RUNS // 2] + elapsed[(RUNS // 2) + 1]) / 2)

if __name__ == '__main__':
    main()
