#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void handle_error (int code, char *msg) {
  if (code < 0) {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[]) {
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  int fd;
  char *buffer = (char *) malloc(mem_size);
  memset(buffer, '0', mem_size);
  char *fifo_name = "./mkfifo";
  char confirmation_char;
  double avg_time = 0.0;
  handle_error(mkfifo(fifo_name, 0666), "\n-->Error en mkfifo()");
  clock_t begin, end;
  for (int i = 0; i < 10; i++) {
    begin = clock();
    handle_error(fd = open(fifo_name, O_WRONLY), "\n-->Error en open()");
    handle_error(write(fd, buffer, mem_size), "\n-->Error en write()");
    close(fd);
    handle_error(fd = open(fifo_name, O_RDONLY), "\n-->Error en open()");
    handle_error(read(fd, &confirmation_char, sizeof(char)), "\n-->Error en read()");
    close(fd);
    end = clock();
    avg_time += (double) (end - begin) / CLOCKS_PER_SEC;
  }
  printf("\n<-----MKFIFO----->");
  printf("\nTamanio de los datos: %lu\nTiempo promedio: %f", mem_size, avg_time / 10);
  free(buffer);
  return 0;
}

