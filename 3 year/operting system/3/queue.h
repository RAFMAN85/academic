
#ifndef UNTITLED1_QUEUE_H
#define UNTITLED1_QUEUE_H
#include <sys/types.h>

const key_t mess_id = 10228;

struct Message {
    long type;
    char text[4096];
};

#endif //UNTITLED1_QUEUE_H
