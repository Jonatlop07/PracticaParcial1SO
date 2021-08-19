#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  FILE *in;
  in = fopen("data.txt", "r");
  if (in == NULL) {
    printf("Error leyendo archivo\n");
    exit(-1);
  }
  char *buffer = (char *) malloc(mem_size);
  fscanf(in, "%s", buffer);
  free(buffer);
  fclose(in);
  return 0;
}
