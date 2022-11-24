#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "holder.h"



int main()
{

    int mem_ds, sem_ds;
    void *mem;
    char *buffer=NULL;
    struct sembuf lock_client[]={{0,-10,0}};
    struct sembuf lock_client2[]={{0,-12,0}};
    struct sembuf unlock_server1[]={{0,+11,0}};
    struct sembuf unlock_server2[]={{0,+13,0}};

    printf("Client 2 started! \n");


    while (1) {
        sem_ds = semget(key, 0, 0);
        if (sem_ds < 0)
            perror("Failed to open semaphore");
        else
            break;

        sleep(1);
    }

    mem_ds = shmget(key, 0, 0);
    if (mem_ds < 0) {
        perror("Failed to create Shared memory");
        return 2;
    }

    mem = shmat(mem_ds, NULL, 0);
    if (mem == (void*) -1) {
        perror("Can't attach");
        return 3;
    }
    semop(sem_ds, lock_client, 1);

    FILE *file_stream2;
    buffer=malloc(4096);
    file_stream2= popen("find . -maxdepth 1 -iname '*.c' | tr '\n' ';'","r");
    fread(buffer,1,4096,file_stream2);
    pclose(file_stream2);
    strcpy(mem, buffer);
    fprintf(stdout, "%s", buffer);

    semop(sem_ds,unlock_server1,1);
    semop(sem_ds, lock_client2, 1);
    printf("\n");
    printf("files more then 2 blocks:\n");
    strcpy(buffer,mem);
    printf("%s\n",buffer);
    printf("\n");
    semop(sem_ds,unlock_server2,1);

    shmdt(mem);
    return 0;
}