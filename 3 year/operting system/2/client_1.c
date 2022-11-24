#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "holder.h"

int main()
{

    int mem_ds, sem_ds;
    void *mem;
    char *buffer=NULL;
    struct sembuf unlock_server[] = {{0, +1, 0}};
    struct sembuf lock_client2[]={{0,-3,0}};
    struct sembuf unlock_server1[]={{0,+2,0}};
    struct sembuf unlock_server2[]={{0,+4,0}};

    printf("Client 1 started! \n");

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
        perror("Failed to create ROP");
        return 2;
    }

    mem = shmat(mem_ds, NULL, 0);
    if (mem == (void*)-1) {
        perror("Can't attach");
        return 3;
    }


    semop(sem_ds,unlock_server1,1);
    FILE *file_stream1;
    buffer=malloc(4096);
    file_stream1= popen("find . -maxdepth 1 -iname '*.txt'| tr '\n' ';'","r");
    fread(buffer,1,4096,file_stream1);
    pclose(file_stream1);

    strcpy(mem,buffer);
    printf("%s", buffer);


    semop(sem_ds, unlock_server, 1);
    semop(sem_ds,lock_client2,1);
    printf("\n");
    printf("files more then 2 blocks\n");
    strcpy(buffer,mem);
    fprintf(stdout, "%s\n",buffer);
    printf("\n");

    semop(sem_ds,unlock_server2,1);

    shmdt(mem);
    struct shmid_ds buf_t;
    time_t correct_time;
    char time_string[20];
    struct tm* needed_time=NULL;
    shmctl(mem_ds,IPC_STAT,&buf_t);

    correct_time=buf_t.shm_dtime;
    needed_time=localtime(&correct_time);
    printf("Last disconnection:\n");
    strftime(time_string,20,"%Y-%m-%d %H:%M:%S", needed_time);
    printf("%s \n", time_string);
    printf("test: ");
    printf("%ld", buf_t.shm_dtime);
    return 0;
}