#include <stdio.h>
#include <sys/socket.h>
#include "head.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

int main(){

    int sid;
    struct sockaddr_un server;
    struct sockaddr_un client;
    memset(&client,0, sizeof(struct sockaddr_un));
    char *message = malloc(MS_SIZE);
    char *answer = malloc(MS_SIZE);
    FILE *file_desc;
    int all_files, dot_files;

    if ((sid=socket(PF_UNIX,SOCK_DGRAM,0))==-1){
        fprintf(stderr, "socket creation failed \n");
    }
    printf("socket id: %d \n", sid);
    client.sun_family=PF_UNIX;
    strcpy(client.sun_path,CLIENT_SOCK);

    server.sun_family=PF_UNIX;
    strcpy(server.sun_path,SERVER_SOCK);

    if( bind(sid,(struct sockaddr *)&client,sizeof(client))==-1){
        fprintf(stderr, "bind client failed \n");
    }

    recvfrom(sid,message,MS_SIZE,0, (struct sockaddr *)&client, (socklen_t*) sizeof(client));
    printf("recieved message: %s \n", message);

    all_files=atoi(message);

    file_desc=popen("find . -maxdepth 1 -name '.*'  | wc -l","r");
    fread(answer,1,MS_SIZE,file_desc);
    pclose(file_desc);
    dot_files=atoi(answer);

    int x=all_files-dot_files;
    char buf_str[MS_SIZE];
    sprintf(buf_str,"%d",x);

    sendto(sid,buf_str,MS_SIZE,0, (struct sockaddr *)&server, sizeof(server));

    shutdown(sid,2);
    close(sid);
    unlink(CLIENT_SOCK);// delete socket
    return 0;
}

