#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"


int main()
{
    int mq_id = msgget(mess_id, IPC_CREAT | 0600);
    if (mq_id == -1) {
        fprintf(stderr, "Failed to create queue with id=%u.\n", mess_id);
        return 1;
    }

    struct Message first, second;
    FILE * file_stream1, * file_stream2;

    if ((file_stream1 = popen("pwd", "r")) == NULL) {
        fprintf(stderr, "Failed to open pipe.");
        return 2;
    }
    fgets(first.text, 4096, file_stream1);
    if (pclose(file_stream1)) {
        fprintf(stderr, "Pipe is closed.");
        return 3;
    }
    first.type = 1;
    msgsnd(mq_id, &first, strlen(first.text) + 1, IPC_NOWAIT);

    if ((file_stream2 = popen("find . -maxdepth 1 -type f -print0 | xargs -0 grep 'define' -l | cut -c 3- | tr '\n' '#' ", "r")) == NULL) {
        fprintf(stderr, "Failed to open pipe.");
        return 4;
    }
    fgets(second.text, 4096, file_stream2);
    if (pclose(file_stream2)) {
        fprintf(stderr, "Pipe is closed.");
        return 5;
    }
    second.type = 2;
    msgsnd(mq_id, &second, strlen(second.text) + 1, IPC_NOWAIT);


    return 0;
}