#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  for (int i = 0; i < 10; i++) {
    FILE *out;
    out = fopen("data.txt", "w");
    if (out == NULL) {
      printf("Error creando archivo\n");
      exit(-1);
    }
    char *buffer = (char *) malloc(mem_size);
    memset(buffer, '0', mem_size);
    fprintf(out, "%s", buffer);
    free(buffer);
    fclose(out);
    system("./cons")
  }
  return 0;
}
