#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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
  int fd;
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  double avg_time = 0.0;
  //for (int i = 0; i < 10; i++) {
  clock_t begin, end;
  char *buffer = (char *)malloc(mem_size);
  memset(buffer, '0', mem_size);
  begin = clock();
  handle_error(fd = open("data.txt", O_RDWR | O_CREAT, 0666), "Fallo al abrir archivo->");
  handle_error(fcntl(fd, F_SETLK, &lock), "Fallo en fcntl->");
  write(fd, buffer, mem_size);
  end = clock();
  avg_time += (double)(end - begin) / CLOCKS_PER_SEC;
  free(buffer);
  close(fd);
  printf("\n%d-%f", mem_size, avg_time);
  //}
  return 0;
}
