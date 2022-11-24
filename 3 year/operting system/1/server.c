#include <stdio.h>
#include <sys/socket.h>
#include "head.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

int main(){
    int sid;
    struct sockaddr_un server,client;
    memset(&server,0, sizeof(struct sockaddr_un));
    memset(&client,0, sizeof(struct sockaddr_un));
    char* buffer = malloc(MS_SIZE);
    char* buffer2 = malloc(MS_SIZE);
    FILE * file_desc;
    char *message = malloc(MS_SIZE);




    if((sid=socket(PF_UNIX,SOCK_DGRAM,0))==-1){
        fprintf(stderr, "socket creation failed \n");
    }
    printf("socket id: %d \n",sid);


    server.sun_family=PF_UNIX;
    strcpy(server.sun_path,SERVER_SOCK);

    client.sun_family=PF_UNIX;
    strcpy(client.sun_path,CLIENT_SOCK);


    unlink(SERVER_SOCK);
    if(bind(sid,(struct sockaddr *)&server,sizeof(server))==-1){
        fprintf(stderr, "bind failed \n");
    }

    file_desc = popen("find . -maxdepth 1  | wc -l","r");
    fread(buffer,1,MS_SIZE,file_desc);
    pclose(file_desc);

    strcpy(message,buffer);
    printf("message send: %s \n",message);
    //memset(message,0,MS_SIZE);

    while((sendto(sid, message, MS_SIZE, 0, (struct sockaddr *) &client, sizeof(client))) == -1) {
        sleep(1);
    }

    memset(message,0,MS_SIZE);

    recvfrom(sid,message,MS_SIZE,0, (struct sockaddr *)&server, (socklen_t *) sizeof(server));

    printf("message recieved: %s \n", message);

    free(buffer);
    free(buffer2);
    free(message);
    shutdown(sid,2);
    close(sid);
    unlink(SERVER_SOCK);
    return 0;
}

