import os
import random
import subprocess
from tqdm import tqdm

# Configuration
EXECUTABLES = ["exception", "returnglobal", "returntype", "returntypec"]
TEST_CASES = 100  # Number of test cases to generate
MAX_TIMES = 1000000  # Maximum value for `times`
MAX_EPERIOD = 100000  # Maximum value for `eperiod`
MAX_SEED = 100000  # Maximum value for `seed`
RESULTS_DIR = "test_results"

# Ensure results directory exists
os.makedirs(RESULTS_DIR, exist_ok=True)


def generate_test_case():
    """Generates a random test case."""
    times = random.randint(1, MAX_TIMES)
    eperiod = random.randint(1, MAX_EPERIOD)
    seed = random.randint(1, MAX_SEED)
    return f"{times} {eperiod} {seed}"


def run_executable(executable, test_input):
    """Runs the executable with the given test input and captures output."""
    command = f"./{executable} {test_input}"
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"[ERROR] {executable} failed with input: {test_input}")
            print(result.stderr)
            return None
        return result.stdout.strip()
    except Exception as e:
        print(f"[ERROR] Exception while running {executable}: {e}")
        return None


def main():
    # Generate and test cases with tqdm progress bar
    for i in tqdm(range(TEST_CASES), desc="Testing Progress", unit="test"):
        # Generate a random test case
        test_input = generate_test_case()

        # Store outputs for comparison
        outputs = {}

        for exe in EXECUTABLES:
            output = run_executable(exe, test_input)
            if output is None:
                print(f"[ERROR] Failed to run {exe} for input {test_input}")
                continue

            # Save the output to the results directory
            output_file = os.path.join(RESULTS_DIR, f"{exe}_{i + 1}.txt")
            with open(output_file, "w") as f:
                f.write(output)

            outputs[exe] = output

        # Compare outputs
        reference_output = outputs.get(EXECUTABLES[0])
        for exe in EXECUTABLES[1:]:
            if outputs.get(exe) != reference_output:
                print(f"[ERROR] Output mismatch for test case {i + 1}")
                print(f"Reference ({EXECUTABLES[0]}):\n{reference_output}")
                print(f"Mismatch ({exe}):\n{outputs.get(exe)}")
                break


if __name__ == "__main__":
    main()
