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
  double avg_time = 0.0;
  
  printf("\n\n<-----PIPES----->\n");

  handle_error(pipe(prod_fd) < 0, "\n-->Error en pipe()"); 
  handle_error(pipe(cons_fd) < 0, "\n-->Error en pipe()");
  handle_error(process_id = fork(), "\n-->Error en fork()");
    
  //for (int i = 0; i < 10; i++) {
    if (process_id == 0) {
      result = read(prod_fd[READ], buffer_cons, mem_size);
      if (result != mem_size) {
	printf("\n-->Datos incompletos en consumidor");
	exit(-1);
      }
      handle_error(write(cons_fd[WRITE], &cons_conf, sizeof(char)), "\n-->Error en write() del consumidor");
    } else {
      begin = clock();
      close(prod_fd[READ]);
      close(cons_fd[WRITE]);
      handle_error(write(prod_fd[WRITE], buffer_prod, mem_size), "\n-->Error en write() del productor");
      result = read(cons_fd[READ], &prod_conf, sizeof(char));
      if (result != sizeof(char)) {
	printf("\n-->Datos incompletos en productor");
	exit(-1);
      }
      end = clock();
      avg_time += (double) (end - begin) / CLOCKS_PER_SEC;
      printf("\nTiempo promedio: %f", mem_size, avg_time);
      close(prod_fd[WRITE]);
      close(cons_fd[READ]);
    }
  //}
  
  free(buffer_prod);
  free(buffer_cons);

  return 0;
}
