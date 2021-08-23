#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
  char *fifo1_name = "./mkfifo1";
  char *fifo2_name = "./mkfifo2";
  char confirmation_char = '1';
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < sizes[i]; k++) {
	handle_error(fd = open(fifo1_name ,O_RDONLY), "\n-->Error en open()");
	handle_error(read(fd, buffer, mem_size), "\n-->Error en read()");
	close(fd);
	handle_error(fd = open(fifo2_name, O_WRONLY), "\n-->Error en open()");
	handle_error(write(fd, &confirmation_char, sizeof(char)), "\n-->Error en write()");
	close(fd);
      }
    }
  }
  free(buffer);
  return 0;
}
