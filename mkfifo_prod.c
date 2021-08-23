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
  int sizes[] = {1, 10, 100, 1024, 1024 * 10, 1024 * 100};
  size_t mem_size = 1024 * sizeof(char);
  int fd;
  char *buffer = (char *) malloc(mem_size);
  memset(buffer, '0', mem_size);
  char *fifo1_name = "./mkfifo1";
  char *fifo2_name = "./mkfifo2";
  char confirmation_char;
  double avg_time = 0.0;
  clock_t begin, end;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 10; j++) {
      begin = clock();
      for (int k = 0; k < sizes[i]; k++) {
	handle_error(fd = open(fifo1_name, O_WRONLY), "\n-->Error en open()");
	handle_error(write(fd, buffer, mem_size), "\n-->Error en write()");
	close(fd);
	handle_error(fd = open(fifo2_name, O_RDONLY), "\n-->Error en open()");
	handle_error(read(fd, &confirmation_char, sizeof(char)), "\n-->Error en read()");
	close(fd);
      }
      end = clock();
      avg_time += (double) (end - begin) / CLOCKS_PER_SEC;
    }
    printf("\nDatos enviados: %ld. Tiempo promedio: %f s", mem_size * sizes[i], avg_time / 10);
  }
  free(buffer);
  return 0;
}

