#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3535

void handle_error (int code, char *msg) {
  if (code < 0) {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[]) {
  size_t mem_size = atoi(argv[2]) * sizeof(char);
  int clientfd;
  struct sockaddr_in client;
  struct hostent *he;
  char *buffer = (char *) malloc(mem_size);
  char confirmation_char = '1';

  handle_error(clientfd = socket(AF_INET, SOCK_STREAM, 0), "\nError en socket()");

  client.sin_family = AF_INET;
  client.sin_port = htons(PORT);
 
  inet_aton(argv[1], &client.sin_addr);
  
  handle_error(
    connect(clientfd, (struct sockaddr *) &client, (socklen_t) sizeof(struct sockaddr)),
    "\n-->Error en connect()"
  );

  handle_error(recv(clientfd, buffer, mem_size, 0), "\n-->Error en recv()");
  printf("\n Rec: %s", buffer);
  handle_error(send(clientfd, &confirmation_char, mem_size, 0), "\n-->Error en send()");
  close(clientfd);

  free(buffer);
  return 0;
}
