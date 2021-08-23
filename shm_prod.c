#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>

void handle_error(int code, char *msg)
{
    if (code < 0)
    {
        perror(msg);
        exit(-1);
    }
}
//Global variables for the shared memory comunication
int memoryId;
key_t key = 1234;
#ifndef MAX
#define MAX 100
#endif

int main(int argc, char *argv[])
{
    clock_t begin, end;
    double avgTime = 0.0;
    size_t memSize = atoi(argv[1]) * sizeof(char);
    begin = clock();
    handle_error( memoryId = shmget(key, memSize, 0666 | IPC_CREAT),"Fallo en el shmget->");
    char *memory = (char *)shmat(memoryId, 0, 0);
    if (memory == NULL)
    {
        printf("\nfallo en el shmat\n");
        return 1;
    }
    memset(memory, '0', memSize);
    char buffer[MAX];
    strcat(strcpy(buffer, "./shm_cons "), argv[1]);
    system(buffer);
    shmdt((char *)memory);
    shmctl(memoryId, IPC_RMID, (struct shmid_ds *)NULL);
    end = clock();
    avgTime += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nDatos enviados: %ld bytes. Tiempo promedio: %f s", memSize, avgTime / 10);
    return 0;
}
