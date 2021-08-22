#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define READ 0
#define WRITE 1

void handle_error (int code, char *msg) {
  if (code < 0) {
    perror(msg);
    exit(-1);
  }
}

int main (int argc, char *argv[]) {
  int result;
  size_t mem_size = atoi(argv[1]) * sizeof(char);
  int prod_fd[2], cons_fd[2];
  char *buffer_prod = (char *) malloc(mem_size);
  memset(buffer_prod, '0', mem_size);
  char *buffer_cons = (char *) malloc(mem_size);
  char prod_conf, cons_conf = '1';
  pid_t process_id;
  clock_t begin, end;
  begin = clock();

  handle_error(pipe(prod_fd) < 0, "\n-->Error en pipe()"); 
  handle_error(pipe(cons_fd) < 0, "\n-->Error en pipe()");
  handle_error(process_id = fork(), "\n-->Error en fork()");
  
  if (process_id == 0) {
    result = read(prod_fd[READ], buffer_cons, mem_size);
    if (result != mem_size) {
      perror("\n-->Datos incompletos en consumidor");
      exit(-1);
    }
    handle_error(write(cons_fd[WRITE], &cons_conf, sizeof(char)), "\n-->Error en write() del consumidor");
  } else {
    close(prod_fd[READ]);
    close(cons_fd[WRITE]);
    handle_error(write(prod_fd[WRITE], buffer_prod, mem_size), "\n-->Error en write() del productor");
    result = read(cons_fd[READ], &prod_conf, sizeof(char));
    if (result != sizeof(char)) {
      perror("\nDatos incompletos en productor");
      exit(-1);
    }
    end = clock();
    printf("\nTime: %f", (double) (end - begin) / CLOCKS_PER_SEC);
  }
  
  free(buffer_prod);
  free(buffer_cons);

  return 0;
}
