#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

void handle_error(int code, char *msg) {
  if (code < 0) {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[])
{
    size_t memSize = atoi(argv[1])  * sizeof(char);
    // variables for the shared memory comunication
    key_t key = 1234;
    int memoryId;
    char *memory = NULL;

    //listen shared Memory
    do
    {
        handle_error(memoryId = shmget(key, memSize, 0666),"Fallo en shmget ->");
    } while (memoryId == -1);

    memory = (char *)shmat(memoryId, 0, 0);

    if (memory == NULL)
    {
        printf("\nfallo en shmat\n");
        exit(0);
    }
    printf(" dato en memory es : %s \n ",memory);

    if (memoryId != -1)
    {
        shmdt((char *)memory);
    }
}