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
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  int fd;
  char *buffer = (char *) malloc(mem_size);
  char *fifo_name = "./mkfifo";
  char confirmation_char = '1';
  //handle_error(mkfifo(fifo_name, 0666), "\n-->Error en mkfifo()");
  //while (1) {
    handle_error(fd = open(fifo_name ,O_RDONLY), "\n-->Error en open()");
    handle_error(read(fd, buffer, mem_size), "\n-->Error en read()");
    printf("\nMsg: %s", buffer);
    close(fd);
    handle_error(fd = open(fifo_name, O_WRONLY), "\n-->Error en open()");
    handle_error(write(fd, &confirmation_char, sizeof(char)), "\n-->Error en write()");
    close(fd);
  //}
  return 0;
}
