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

int main () {
  int result;
  int sizes[] = {1, 10, 100, 1024, 1024 * 10, 1024 * 100};
  size_t mem_size = 1024 * sizeof(char);
  int prod_fd[2], cons_fd[2];
  char *buffer_prod = (char *) malloc(mem_size);
  memset(buffer_prod, '0', mem_size);
  char *buffer_cons = (char *) malloc(mem_size);
  char prod_conf, cons_conf = '1';
  pid_t process_id;
  clock_t begin, end;
  double avg_time = 0.0;

  handle_error(pipe(prod_fd) < 0, "\n-->Error en pipe()"); 
  handle_error(pipe(cons_fd) < 0, "\n-->Error en pipe()");
  handle_error(process_id = fork(), "\n-->Error en fork()");
    
  if (process_id == 0) {
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 10; j++) {
	for (int k = 0; k < sizes[i]; k++) {
	  result = read(prod_fd[READ], buffer_cons, mem_size);
	  if (result != mem_size) {
	  printf("\n-->Datos incompletos en consumidor");
	    exit(-1);
	  }
	  handle_error(write(cons_fd[WRITE], &cons_conf, sizeof(char)), "\n-->Error en write() del consumidor");
	}
      }
    }
  } else {
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 10; j++) {
	begin = clock();
	for (int k = 0; k < sizes[i]; k++) {
	  handle_error(write(prod_fd[WRITE], buffer_prod, mem_size), "\n-->Error en write() del productor");
	  result = read(cons_fd[READ], &prod_conf, sizeof(char));
	  if (result != sizeof(char)) {
	    printf("\n-->Datos incompletos en productor");
	    exit(-1);
	  }
	}
	end = clock();
	avg_time += (double) (end - begin) / CLOCKS_PER_SEC;
      }
      printf("\nDatos enviados: %ld. Tiempo promedio: %f s", mem_size * sizes[i], avg_time / 10);
    }
  }
  
  free(buffer_prod);
  free(buffer_cons);

  return 0;
}
