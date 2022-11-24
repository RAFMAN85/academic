#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "holder.h"

int main()
{
    int mem_ds, sem_ds, res;
    char *mem;
    struct sembuf lock_server[] = {{0, -2, 0}};
    struct sembuf lock_server1[]={{0,-4,0}};
    struct sembuf lock_server2[]={{0,-11,0}};
    struct sembuf lock_server3[]={{0,-13,0}};
    struct sembuf unlock_client[]={{0,+1,0}};
    struct sembuf unlock_client1[]={{0,+3,0}};
    struct sembuf unlock_client2[]={{0,+10,0}};
    struct sembuf unlock_client3[]={{0,+12,0}};
    struct shmid_ds shm_buf;
    struct stat sb;


    sem_ds = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_ds < 0) {
        perror("Failed to create semaphore");
        return 1;
    }

    mem_ds = shmget(key, MAX_SIZE, IPC_EXCL | IPC_CREAT | 0600);
    if (mem_ds < 0) {
        perror("Failed to create Shared memory");
        res = 2;
        semctl(sem_ds, 0, IPC_RMID);
        return res;
    }

    printf("shared memory was created\n");
    semctl(sem_ds, 0, SETALL,0);

    mem = shmat(mem_ds, NULL, 0);
    if (mem == (void*)-1) {
        perror("Can't attach");
        res = 3;
        shmctl(mem_ds, IPC_RMID, NULL);
        return res;
    }
    semop(sem_ds, lock_server, 1);

    semop(sem_ds,unlock_client,1);
    semop(sem_ds,lock_server,1);
    //do something for client 1
    char data_c_1[4096];
    char * file_array_1[16];
    strcpy(data_c_1,mem);
    char * current_1 = strtok(data_c_1,";");
    int idx1=0;
    memset(mem,0,4096);
    char* buf_string_1;
    while(current_1 != NULL){
        stat(current_1,&sb);
        if(sb.st_blocks>=2) {
            file_array_1[idx1] = current_1;
            idx1++;
        }

        current_1=strtok(NULL,";");
    }
    buf_string_1= arr_unite(idx1,file_array_1,"\n");
    strcpy(mem, buf_string_1);
    free(buf_string_1);

    semop(sem_ds,unlock_client1,1);
    semop(sem_ds,lock_server1,1);
    //client1 is running
    semop(sem_ds,unlock_client2,1);
    semop(sem_ds,lock_server2,1);
    //do something for client 2
    char data_c_2[4096];
    char * file_array[16];
    strcpy(data_c_2,mem);
    char * current = strtok(data_c_2,";");
    int idx=0;
    memset(mem,0,4096);
    char* buf_string;
    while(current != NULL){
        stat(current,&sb);
        if(sb.st_blocks>=2){
            file_array[idx]= current;
            idx++;
        }
        current=strtok(NULL,";");
    }

    buf_string= arr_unite(idx,file_array,"\n");
    strcpy(mem, buf_string);
    free(buf_string);

    semop(sem_ds,unlock_client3,1);
    semop(sem_ds,lock_server3,1);
    //client 2 is running


    shmctl(mem_ds, IPC_STAT, &shm_buf);

    shmctl(mem_ds, IPC_RMID, NULL);
    semctl(sem_ds, 0, IPC_RMID);

    return res;
}