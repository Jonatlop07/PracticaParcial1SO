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

int main(int argc, char *argv[])
{
    clock_t begin, end;
    double avgTime = 0.0;
    size_t memSize = atoi(argv[1]) * sizeof(char);
    handle_error( memoryId = shmget(key, memSize, 0666 | IPC_CREAT),"Fallo en el shmget->");
    char *memory = (char *)shmat(memoryId, 0, 0);
    if (memory == NULL)
    {
        printf("\nfallo en el shmat\n");
        return 1;
    }
    begin = clock();
    memset(memory, '0', memSize);
    end = clock();
    avgTime += (double)(end - begin) / CLOCKS_PER_SEC;
    system("./shm_cons 1024");
    printf("\n%ld-%f", memSize, avgTime);
    shmdt((char *)memory);
    shmctl(memoryId, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
