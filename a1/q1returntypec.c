#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { RESULT_DOUBLE, RESULT_EX1, RESULT_EX2, RESULT_EX3 } ResultType;

typedef struct {
  ResultType type;
  union {
    double value;
    short int ex1_code;
    int ex2_code;
    long int ex3_code;
  };
} Result;

intmax_t eperiod = 10000;
int randcnt = 0;
int Rand() {
  randcnt += 1;
  return rand();
}

Result rtn1(double i) {
  if (Rand() % eperiod == 0) {
    Result result = {.type = RESULT_EX1, .ex1_code = (short int)Rand()};
    return result;
  }
  Result result = {.type = RESULT_DOUBLE, .value = i + Rand()};
  return result;
}

Result rtn2(double i) {
  if (Rand() % eperiod == 0) {
    Result result = {.type = RESULT_EX2, .ex2_code = Rand()};
    return result;
  }
  Result res1 = rtn1(i);
  if (res1.type != RESULT_DOUBLE)
    return res1;
  Result result = {.type = RESULT_DOUBLE, .value = res1.value + Rand()};
  return result;
}

Result rtn3(double i) {
  if (Rand() % eperiod == 0) {
    Result result = {.type = RESULT_EX3, .ex3_code = Rand()};
    return result;
  }
  Result res2 = rtn2(i);
  if (res2.type != RESULT_DOUBLE)
    return res2;
  Result result = {.type = RESULT_DOUBLE, .value = res2.value + Rand()};
  return result;
}

int main(int argc, char *argv[]) {
  intmax_t times = 100000000, seed = getpid();
  int arg_error = 0;

  // Multi-level exit for command-line argument parsing
  do {
    if (argc > 1) {
      times = strtoll(argv[1], NULL, 10);
      if (times <= 0) {
        arg_error = 1;
        break;
      }
    }
    if (argc > 2) {
      eperiod = strtoll(argv[2], NULL, 10);
      if (eperiod <= 0) {
        arg_error = 1;
        break;
      }
    }
    if (argc > 3) {
      seed = strtoll(argv[3], NULL, 10);
      if (seed <= 0) {
        arg_error = 1;
        break;
      }
    }
  } while (0);

  if (arg_error) {
    fprintf(stderr,
            "Usage: %s [times > 0 | d [eperiod > 0 | d [seed > 0 | d]]]\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  srand(seed);

  double rv = 0.0;
  int ev1 = 0, ev2 = 0, ev3 = 0;
  int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

  for (int i = 0; i < times; i++) {
    Result res = rtn3((double)i);
    switch (res.type) {
    case RESULT_DOUBLE:
      rv += res.value;
      rc++;
      break;
    case RESULT_EX1:
      ev1 += res.ex1_code;
      ec1++;
      break;
    case RESULT_EX2:
      ev2 += res.ex2_code;
      ec2++;
      break;
    case RESULT_EX3:
      ev3 += res.ex3_code;
      ec3++;
      break;
    }
  }

  printf("randcnt %d\n", randcnt);
  printf("normal result %g exception results %d %d %d\n", rv, ev1, ev2, ev3);
  printf("calls %d exceptions %d %d %d\n", rc, ec1, ec2, ec3);

  return 0;
}
