import subprocess
import random
import string
import os

# Paths to the executables
FILTER_EXEC = "./filter"
FILTER_SAMPLE_EXEC = "./filter_sample"

# List of possible filter flags
FILTER_FLAGS = ["-h", "-w"]

os.environ["NOSUMMARY"] = "1"
os.makedirs("test_results", exist_ok=True)


# Function to generate random input text
def generate_random_input():
    length = random.randint(500, 1000)
    return ''.join(random.choice(string.ascii_letters + string.digits + string.punctuation + ' \t\n') for _ in range(length))


# Function to generate a random list of filter flags
def generate_random_flags():
    num_flags = random.randint(2, 8)  # Randomly choose 0 to 3 flags
    return random.choices(FILTER_FLAGS, k=num_flags)


# Function to run a command and capture its output
def run_command(executable, flags, input_text):
    command = [executable] + flags
    process = subprocess.Popen(command, stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate(input=input_text)
    return stdout, stderr


# Function to compare outputs and print results
def compare_outputs(filter_output, sample_output, flags, input_text):
    if filter_output == sample_output:
        print("✅ Test passed!")
    else:
        print("❌ Test failed!")
        print("Flags:", flags)
        print("Input:", repr(input_text))
        print("Filter output:", repr(filter_output))
        print("Sample output:", repr(sample_output))


# Main testing function
def test_filter(num_tests):
    for i in range(num_tests):
        # Generate random input and flags
        input_text = generate_random_input()
        with open(f"test_results/q1.in{i+1}", "w") as f:
            f.write(input_text)
        flags = generate_random_flags()
        print(f"Running test {i + 1}/{num_tests}, flags: {flags}")
        with open(f"test_results/q1.flags{i+1}", "w") as f:
            f.write(" ".join(flags))

        # Run both executables
        filter_output, filter_stderr = run_command(FILTER_EXEC, flags, input_text)
        with open(f"test_results/q1.out{i+1}", "w") as f:
            f.write(filter_output)
        sample_output, sample_stderr = run_command(FILTER_SAMPLE_EXEC, flags, input_text)
        with open(f"test_results/q1.expect{i+1}", "w") as f:
            f.write(sample_output)

        # Compare outputs
        compare_outputs(filter_output, sample_output, flags, input_text)

        # Print stderr if there are errors
        if filter_stderr:
            print("Filter stderr:", filter_stderr)
        if sample_stderr:
            print("Sample stderr:", sample_stderr)


# Run the tests
if __name__ == "__main__":
    test_filter(num_tests=50)
