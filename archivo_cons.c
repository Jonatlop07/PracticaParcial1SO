#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void handle_error(int code, char *msg) {
  if (code < 0) {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[]) {
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  lock.l_pid = getpid();
  int fd;
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  //for (int i = 0; i < 10; i++) {
    char *buffer = (char *) malloc(mem_size);
    handle_error(fd = open("data.txt", O_RDONLY), "Fallo al abrir para leer->");
    fcntl(fd, F_GETLK, &lock);
    while (lock.l_type != F_UNLCK) {
      fcntl(fd, F_GETLK, &lock);
    }
      //handle_error(-1, "Fallo al obtener un bloqueo de solo lectura->");
    char c;
    read(fd, buffer, mem_size);
    printf("\nMem_size: %d", mem_size);
    lock.l_type = F_UNLCK;
    handle_error(fcntl(fd, F_SETLK, &lock), "Fallo en desbloqueo explicito->");
    free(buffer);
    close(fd);
  //}
  return 0;
}
