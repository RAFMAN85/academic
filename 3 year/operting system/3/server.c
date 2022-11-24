#include "queue.h"
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

struct file{
    time_t time;
    char * file_name;
};



int main(){

    int queue=msgget(mess_id,0);
    if (queue == -1) {
        fprintf(stderr, "Failed open message queue with id=%u.\n", mess_id); // check fprintf
        return 1;
    }

    struct Message message2;
    struct Message message1;

    if (msgrcv(queue, &message2, 4096, 2, IPC_NOWAIT) == -1) {
        fprintf(stderr, "Failed to get second message from message queue.\n");
        return 2;
    }

    if (msgrcv(queue, &message1, 4096, 1, IPC_NOWAIT) == -1){
        fprintf(stderr, "Failed to get first message from message queue.\n");
        return 1;
    }

    char files[4096];
    strcpy(files, message2.text);

    struct file file_array[16];

    struct stat attrib;

    struct file current_file;

    FILE * file_stream;
    char * current_filename = strtok(files,"#");
    int idx=0;
    while (current_filename != NULL){
       stat(current_filename, &attrib);
       current_file.file_name=current_filename;
       current_file.time = attrib.st_mtime;
       file_array[idx]=current_file;
       current_filename = strtok(NULL,"#");
       idx++;
    }

    //sorting(file_array,idx);

    struct file temp;
    for (int i=0; i<idx; i++){
        for (int j=0; j<idx; j++){
            if(file_array[j-1].time>file_array[j].time){
                temp=file_array[j-1];
                file_array[j-1]=file_array[j];
                file_array[j]=temp;
            }
        }
    }

    time_t correct_time;
    char time_string[20];
    struct tm* needed_time=NULL;

    printf("%s", message1.text);

    for(int i=0;i<idx;i++) {
        printf("Filename: %s \n", file_array[i].file_name);
        correct_time=file_array[i].time;
        needed_time= localtime(&correct_time);
        strftime(time_string,20,"%Y-%m-%d %H:%M:%S", needed_time);
        printf("%s \n", time_string);
    }

    struct msqid_ds mq_info;
    if (msgctl(queue, IPC_STAT, &mq_info) == -1) {
        fprintf(stderr, "Failed to get message queue stats with id=%u.\n", queue);
        return -1;
    }

    printf("id of last message sent: %d .\n", mq_info.msg_lspid);
    printf("max queue size in bytes: %lu .\n", mq_info.msg_qbytes);

    if (msgctl(queue, IPC_RMID, 0) == -1) {
        fprintf(stderr, "Failed to delete message queue with id=%u.\n", mess_id);
        return 3;
    } else {
        fprintf(stdout, "Deleted message queue with id=%u.\n", mess_id);
    }
    return 0;
}