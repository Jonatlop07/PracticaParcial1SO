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
  handle_error(mkfifo(fifo_name, 0666), "\n-->Error en mkfifo()");
  clock_t begin, end;
  //while (1) {
    handle_error(fd = open(fifo_name, O_WRONLY), "\n-->Error en open()");
    begin = clock();
    handle_error(write(fd, buffer, mem_size), "\n-->Error en write()");
    printf("\nBuff %s", buffer);
    close(fd);
    handle_error(fd = open(fifo_name, O_RDONLY), "\n-->Error en open()");
    handle_error(read(fd, &confirmation_char, sizeof(char)), "\n-->Error en read()");
    close(fd);
    end = clock();
  //}
  return 0;
}

