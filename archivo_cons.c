#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void handle_error(int code, char *msg)
{
  if (code < 0)
  {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[])
{
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  lock.l_pid = getpid();
  int fd,fc;

  size_t mem_size = atoi(argv[1]) * sizeof(char);
  char *buffer = (char *)malloc(mem_size);

  for (int i = 0; i < 10; i++) {
    handle_error(fc = open("check.txt", O_RDWR | O_CREAT, 0666), "Fallo al abrir archivo confirmacion->");
    handle_error(fcntl(fc, F_SETLKW, &lock), "Fallo en fcntl confirmacion->");
    write(fc, "0", sizeof(char));
    handle_error(fd = open("data.txt", O_RDONLY), "Fallo al abrir para leer->");
    fcntl(fd, F_GETLK, &lock);
    while (lock.l_type != F_UNLCK)
    {
      fcntl(fd, F_GETLK, &lock);
    }
    char c;
    read(fd, buffer, mem_size);
    lock.l_type = F_UNLCK;
    handle_error(fcntl(fd, F_SETLK, &lock), "Fallo en desbloqueo explicito->");
    close(fd);
    lseek(fc,0,SEEK_SET);
    write(fc, "1", sizeof(char));
    close(fc);
  }

  free(buffer);
  return 0;
}
