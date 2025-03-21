import subprocess
import random
import os

VOTE_EXE = "./vote"
REFERENCE_EXE = {"INTB": "./voteINTB", "AUTO": "./voteAUTO", "TASK": "./voteTASK"}
VIMPL = ["EXT", "INT", "INTB", "AUTO", "TASK"]


def write_to_file(content, filename):
    """Write content to a file."""
    with open(filename, "w") as f:
        f.write(content)


def build_vote(vimpl):
    print(f"\n🔧 Building vote with VIMPL={vimpl}...\n")
    result = subprocess.run(
        ["make", "vote", f"VIMPL={vimpl}"], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"❌ Build failed for {vimpl}:\n{result.stderr}")
        exit(1)
    print(f"✅ Successfully built vote with {vimpl}\n")


def generate_random_params():
    voters = random.randint(1, 30)
    group = random.randint(1, 30)
    tours = random.randint(1, 50)
    seed = random.randint(1, 1000000000000)
    return voters, group, tours, seed


def run_test(voters, group, tours, seed, mode):
    cmd = [VOTE_EXE, str(voters), str(group), str(tours), str(seed)]
    print(f"▶ Running {mode} vote test: {' '.join(cmd)}")

    # Run vote executable
    result = subprocess.run(cmd, capture_output=True, text=True)

    # Capture stdout and stderr
    output = result.stdout
    error = result.stderr

    return output, error


def run_reference(voters, group, tours, seed, mode):
    cmd = [REFERENCE_EXE[mode], str(voters), str(group), str(tours), str(seed)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.stdout, result.stderr


def validate_tests(test_count=10):
    for vimpl in VIMPL:
        build_vote(vimpl)

        passed_tests = 0
        failed_tests = 0

        for i in range(test_count):
            voters, group, tours, seed = generate_random_params()
            args = f"Voters={voters}, Group={group}, Tours={tours}, Seed={seed}"
            print(f"\n🔍 Test {i+1}/{test_count}: {args}")

            # Run VIMPL version
            output, error = run_test(voters, group, tours, seed, vimpl)

            write_to_file(args, f"test_results/{vimpl}_args_{i+1}.txt")
            write_to_file(output, f"test_results/{vimpl}_output_{i+1}.txt")

            if vimpl in ["MC", "SEM"]:
                if "**** BARGING ERROR ****" in error:
                    print(f"❌ Error in output for test case ({args})")
                    write_to_file(error, f"test_results/{vimpl}_error_{i+1}.txt")
                    failed_tests += 1
                    continue

            if vimpl in REFERENCE_EXE:
                reference_output, _ = run_reference(voters, group, tours, seed, vimpl)
                write_to_file(reference_output, f"test_results/{vimpl}_reference_{i+1}.txt")
                if output != reference_output:
                    print(f"❌ Mismatch in output for test case ({args})")
                    failed_tests += 1
                    continue

            print("✅ Test passed!")
            passed_tests += 1

        print(f"\n🎯 Total Passed: {passed_tests}, Total Failed: {failed_tests}")
        if failed_tests == 0:
            print("🎉 All tests passed successfully!")
        else:
            print("⚠️ Some tests failed. Check logs for details.")


# Run the validation tests
if __name__ == "__main__":
    # Create a directory for test cases
    os.makedirs("test_results", exist_ok=True)
    validate_tests(test_count=20)  # Run 20 randomized tests
