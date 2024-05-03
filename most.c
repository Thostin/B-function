#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

int main(void) {
  FILE *readfile = fopen("primes.bin", "rb");
  if (NULL == readfile) {
    printf("Could not open file \"primes.bin\"\n");
    return 1;
  }
  int lim;
  if (1 != fread(&lim, sizeof(int), 1, readfile)) {
    printf("Could not read 4 bytes from \"primes.bin\"\n");
    return 1;
  }
  if (lim <= 0) {
    printf("File \"primes.bin\" says that there are not numbers in it\n");
    return 1;
  }
  printf("Read all numbers? (y / n) ");
  if (tolower(getchar()) == 'y') {
    int *arr = malloc(sizeof(int) * lim);
    int ret;
    if (lim != (ret = fread(arr, sizeof(int), lim, readfile))) {
      printf("Could only read %d numbers of 4 bytes each from \"primes.bin\" "
             "while %d where espected\n",
             ret, lim);
      return 1;
    }
    for (int i = 0; i < lim; ++i) {
      printf("%d ", arr[i]);
    }
  }
  printf("\n\nTOTAL PRIMES: %d\n", lim);

  return EXIT_SUCCESS;
}
