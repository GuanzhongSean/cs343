import os
import random
import subprocess
from tqdm import tqdm

RESULTS_DIR = "test_results"


def generate_test_cases(num_cases, m_range, n_range, seed_range, eperiod_range):
    """
    Generate random test cases for the two program variants.

    Args:
        num_cases (int): Number of test cases to generate.
        m_range (tuple): Range of m values (min, max).
        n_range (tuple): Range of n values (min, max).
        seed_range (tuple): Range of seed values (min, max).
        eperiod_range (tuple): Range of eperiod values (min, max).

    Returns:
        list: List of test cases in the format (m, n, seed, eperiod).
    """
    test_cases = []
    for _ in range(num_cases):
        m = random.randint(*m_range)
        n = random.randint(*n_range)
        seed = random.randint(*seed_range)
        eperiod = random.randint(*eperiod_range)
        test_cases.append((m, n, seed, eperiod))
    return test_cases


def run_program(executable, test_case):
    """
    Run the given executable with the provided test case.

    Args:
        executable (str): Path to the executable.
        test_case (tuple): Test case parameters (m, n, seed, eperiod).

    Returns:
        str: Output of the executable.
    """
    m, n, seed, eperiod = test_case
    try:
        result = subprocess.run(
            [executable, str(m), str(n), str(seed), str(eperiod)],
            capture_output=True,
            text=True,
            check=True
        )
        return result.stdout
    except subprocess.CalledProcessError as e:
        return e.stdout + e.stderr


def compare_results(throwcatch_exe, longjmp_exe, test_cases):
    """
    Compare results of the two executables for the provided test cases.

    Args:
        throwcatch_exe (str): Path to the throwcatch executable.
        longjmp_exe (str): Path to the longjmp executable.
        test_cases (list): List of test cases.

    Returns:
        None
    """
    for i, test_case in enumerate(tqdm(test_cases, desc="Comparing results")):
        result_throwcatch = run_program(throwcatch_exe, test_case)
        output_file = os.path.join(RESULTS_DIR, f"{throwcatch_exe}_{i + 1}.txt")
        with open(output_file, "w") as f:
            f.write(result_throwcatch)
        result_longjmp = run_program(longjmp_exe, test_case)
        output_file = os.path.join(RESULTS_DIR, f"{longjmp_exe}_{i + 1}.txt")
        with open(output_file, "w") as f:
            f.write(result_longjmp)

        if result_throwcatch != result_longjmp:
            print(f"Mismatch found in test case {i + 1}: {test_case}")
            print(f"ThrowCatch Output:\n{result_throwcatch}")
            print(f"LongJmp Output:\n{result_longjmp}")


if __name__ == "__main__":
    # Configuration
    NUM_TEST_CASES = 100
    M_RANGE = (0, 10)
    N_RANGE = (0, 10)
    SEED_RANGE = (1, 10000)
    EPERIOD_RANGE = (1, 100)

    THROWCATCH_EXECUTABLE = "./throwcatch"
    LONGJMP_EXECUTABLE = "./longjmp"

    # Generate test cases
    test_cases = generate_test_cases(NUM_TEST_CASES, M_RANGE, N_RANGE, SEED_RANGE, EPERIOD_RANGE)

    # Compare results of the two variants
    compare_results(THROWCATCH_EXECUTABLE, LONGJMP_EXECUTABLE, test_cases)
