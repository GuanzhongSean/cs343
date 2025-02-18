import os
import subprocess
import random
import shutil

# Configuration
STYPES = ["double", "int", "string"]
SIMPLS = ["ACTOR", "CBEGIN", "TASK"]
INSTRUCTOR_EXE_PREFIX = "quicksort_"
YOUR_EXE_NAME = "quicksort"
NUM_TEST_CASES = 20  # Number of test cases to generate per configuration
MAX_NUM_LISTS = 100  # Maximum number of lists in the test case
MAX_LIST_SIZE = 100  # Maximum size of each list in the test case
MAX_VALUE = 100000  # Maximum value for numeric types
MAX_STRING_LENGTH = 10  # Maximum length of strings for string type
DEPTH = 2  # Depth parameter for quicksort


def generate_test_case(stype, num_lists=5):
    """Generate a test case file with multiple lists."""
    test_case = []
    for _ in range(num_lists):
        list_size = random.randint(1, MAX_LIST_SIZE)
        if stype == "int":
            values = [random.randint(-MAX_VALUE, MAX_VALUE) for _ in range(list_size)]
        elif stype == "double":
            values = [random.uniform(-MAX_VALUE, MAX_VALUE) for _ in range(list_size)]
        elif stype == "string":
            values = [
                ''.join(random.choices("abcdefghijklmnopqrstuvwxyz",
                        k=random.randint(1, MAX_STRING_LENGTH)))
                for _ in range(list_size)
            ]
        else:
            raise ValueError(f"Unsupported STYPE: {stype}")
        test_case.append(f"{list_size} {' '.join(map(str, values))}")
    return "\n".join(test_case)


def write_to_file(content, filename):
    """Write content to a file."""
    with open(filename, "w") as f:
        f.write(content)


def run_command(command):
    """Run a shell command and return its output."""
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running command: {command}")
        print(result.stderr)
        exit(1)
    return result.stdout.strip()


def compare_outputs(output1, output2):
    """Compare two outputs line by line."""
    lines1 = output1.splitlines()
    lines2 = output2.splitlines()
    if len(lines1) != len(lines2):
        return False
    for l1, l2 in zip(lines1, lines2):
        if l1.strip() != l2.strip():
            return False
    return True


def main():
    # Create a directory for test cases
    os.makedirs("test_results", exist_ok=True)

    # Iterate over all combinations of STYPE and SIMPL
    for stype, simpl in zip(STYPES, SIMPLS):
        print(f"Testing STYPE={stype}, SIMPL={simpl}...")

        # Build your executable
        print("Building your executable...")
        run_command(f"make quicksort STYPE={stype} SIMPL={simpl} OPT=\"-O2 -multi -nodebug\"")

        # Generate test cases
        print("Generating test cases...")
        test_cases = []
        for i in range(NUM_TEST_CASES):
            test_case = f"test_results/test_{stype}_{simpl}_{i+1}"
            test_file = f"{test_case}.in"
            test_content = generate_test_case(stype, random.randint(5, MAX_NUM_LISTS))
            write_to_file(test_content, test_file)
            test_cases.append(test_case)

        # Compare outputs
        for test_case in test_cases:
            print(f"Running test case: {test_case}")

            # Run instructor's executable
            instructor_exe = f"{INSTRUCTOR_EXE_PREFIX}{stype.lower()}_{simpl}"
            instructor_output = run_command(f"./{instructor_exe} {test_case}.in d {DEPTH}")

            # Run your executable
            your_output = run_command(f"./{YOUR_EXE_NAME} {test_case}.in d {DEPTH}")

            # Compare outputs
            if compare_outputs(instructor_output, your_output):
                print(f"✅ Test case {test_case} passed!")
            else:
                print(f"❌ Test case {test_case} failed!")

            # Write outputs to files
            write_to_file(instructor_output, f"{test_case}.expect")
            write_to_file(your_output, f"{test_case}.out")


if __name__ == "__main__":
    main()
