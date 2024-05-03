/*
 * I could use a binary tree for making things faster
 * */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef true
#define true 1
#endif

#ifndef nullptr
#define nullptr (void *)0x0
#endif

// Posible returning value of push
#define zero 2

typedef long type_frac;

// a / b
struct frac {
  type_frac a, b;
};

struct Node {
  struct Node *next;
  type_frac a;
};

void read_primes(void);
type_frac f(type_frac);
int push(type_frac a, struct Node *root);
void B(struct frac f);

int *primes;
int primes_how;

int main(int argc, char *argv[]) {
  if (2 != argc) {
    printf("Improper input\n");
    return 1;
  }
  struct frac p;
  if (2 != sscanf(argv[1], "%ld/%ld", &(p.a), &(p.b)) || p.a <= 0 || p.b <= 0) {
    printf("Improper input.\n");
    return 1;
  }

  read_primes();
  B(p);
  // printf("%ld %ld\n", f(15L), f(8L));

  return EXIT_SUCCESS;
}

void read_primes(void) {
  FILE *fp = fopen("primes.bin", "rb");
  if (NULL == fp) {
    printf("Could not open file primes.bin\n");
    exit(1);
  }
  int ret = fread(&primes_how, sizeof(int), 1, fp);
  if (ret == 0 || primes_how <= 0) {
    printf("No primes readed, exiting now\n");
    exit(1);
  }
  primes = malloc(primes_how * sizeof(int));
  if (NULL == primes) {
    printf("Could not reserve memory for %zu bytes\n",
           primes_how * sizeof(int));
    exit(1);
  }

  ret = fread(primes, sizeof(int), primes_how, fp);
  if (ret != primes_how) {
    printf("Could not read %d primes from primes.bin\n", primes_how);
    free(primes); // I think it is not necessary
    exit(1);
  }
}

type_frac f(type_frac a) {
  if (1 == a)
    return 0;
  type_frac sum = 0;
  long lim = sqrt(a);
  /*
  if (lim > primes_how) {
    printf("We just don't have such amount of primes\n");
    exit(1);
  }*/
  long div;
  long stat;
  char band = 1; // Is prime for now

  for (int i = 0; (stat = primes[i]) <= lim; ++i) {
    div = a / stat;
    if (div * stat == a) {
      a = div;
      sum += stat;
      band = 0;
      while (1) {
        div = a / stat;
        if (div * stat == a) {
          a = div;
          sum += stat;
        } else
          break;
      }
      lim = sqrt(a);
    }
  }

  if (band == 1)
    return a;
  return sum + (a != 1 ? a : 0);
}

/*
 * Se puede optimizar muchísimo ya que el denominador jamás se simplificará
 * */

int push(type_frac a, struct Node *root) {
  if (0 == a)
    return zero;
  while (true) {
    if (root->a == a)
      return true;
    if (nullptr == root->next) {
      root->next = malloc(sizeof(struct Node));
      root = root->next;
      root->a = a;
      return 0;
    }
    root = root->next;
  }
}

void B(const struct frac rat) {
  long steps = 1;
  struct Node root_var;
  const type_frac b = rat.b;
  root_var.a = b;
  root_var.next = nullptr;

  type_frac act_a = rat.a;
  const type_frac f_b = f(b);

  struct Node *root = &root_var;
  while (true) {
    printf("%ld\n", act_a);
    act_a = b * (f(act_a) - f_b) - act_a;
    if (act_a < 0)
      act_a = -act_a;

    switch (push(act_a, root)) {
    case 1:
      printf("%ld\n", act_a);
      printf("Cycles at %ld\n", steps);
      return;
    case 2: // I think this is actually impossible
      printf("Goes down to zero\n");
      return;
    }
    ++steps;
  }
}
