import os
import subprocess
import random

# Define the output directory
output_dir = "test_results"
os.makedirs(output_dir, exist_ok=True)


# Function to generate random test arguments
def generate_test_args():
    # Randomly decide whether to use default values or random values
    use_default_games = random.choice([True, False, False, False, False])
    use_default_players = random.choice([True, False, False, False, False])
    use_default_cards = random.choice([True, False, False, False, False])

    # Generate random values based on constraints
    games = random.randint(0, 100) if not use_default_games else 'd'
    players = random.randint(2, 20) if not use_default_players else 'd'
    cards = random.randint(1, 200) if not use_default_cards else 'd'
    seed = random.randint(1, 1000)

    return [str(games), str(players), str(cards), str(seed)]


# Function to run the executable and capture the output
def run_executable(executable, args):
    result = subprocess.run([executable] + args, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, text=True)
    return result.stdout


# Define the paths to the executables
cardgame_exec = "./cardgame"
cardgame_sample_exec = "./cardgame_sample"


# Generate and run test cases
def test_cardgame(num_tests):
    for test_id in range(1, num_tests + 1):
        # Generate random test arguments
        test_args = generate_test_args()
        print(f"Running test {test_id}/{num_tests}, args: {test_args}")

        # Run both executables and capture their outputs
        output_cardgame = run_executable(cardgame_exec, test_args)
        output_cardgame_sample = run_executable(cardgame_sample_exec, test_args)

        if output_cardgame == output_cardgame_sample:
            print(f"✅ Test case passed!")
        else:
            print(f"❌ Test case failed!")

        # Define the file names
        args_file = os.path.join(output_dir, f"q2.args{test_id}")
        out_file = os.path.join(output_dir, f"q2.out{test_id}")
        expect_file = os.path.join(output_dir, f"q2.expect{test_id}")

        # Write the test arguments to q2.args{#}
        with open(args_file, "w") as f:
            f.write(" ".join(test_args))

        # Write the output of cardgame to q2.out{#}
        with open(out_file, "w") as f:
            f.write(output_cardgame)

        # Write the output of cardgame_sample to q2.expect{#}
        with open(expect_file, "w") as f:
            f.write(output_cardgame_sample)


if __name__ == "__main__":
    test_cardgame(num_tests=50)
