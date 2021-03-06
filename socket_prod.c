#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PORT 3535
#define BACKLOG 2

void handle_error(int code, char *msg)
{
  if (code < 0)
  {
    perror(msg);
    exit(-1);
  }
}

int main()
{
  int sizes[] = {1, 10, 100, 1024, 1024 * 10, 1024 * 100};
  size_t mem_size = 1024 * sizeof(char);
  int opt = 1, serverfd, clientfd;
  struct sockaddr_in server, client;
  struct hostent *he;
  socklen_t socklen;
  char confirmation_char;
  char *buffer = (char *)malloc(mem_size);
  double avg_time = 0.0;
  clock_t begin, end;

  memset(buffer, '0', mem_size);

  handle_error(serverfd = socket(AF_INET, SOCK_STREAM, 0), "\n-->Error en socket()");

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  bzero(server.sin_zero, 8);

  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
  handle_error(
      bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr)),
      "\n-->Error en bind()");
  handle_error(listen(serverfd, BACKLOG), "\n-->Error en listen()");
  handle_error(
      clientfd = accept(serverfd, (struct sockaddr *)&client, &socklen),
      "\n-->Error en accept()");
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      begin = clock();
      for (int k = 0; k < sizes[i]; k++)
      {
        handle_error(send(clientfd, buffer, mem_size, 0), "\n-->Error en send()");
        handle_error(recv(clientfd, &confirmation_char, sizeof(char), 0), "\n-->Error en recv()");
      }
      end = clock();
      avg_time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    printf("\nDatos enviados: %ld bytes. Tiempo promedio: %f s", mem_size * sizes[i], avg_time / 10);
  }
  free(buffer);
  close(clientfd);
  close(serverfd);
  return 0;
}
