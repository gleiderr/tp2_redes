#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "mensagem.h"

void sendMSG(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ, uint16_t length, char* msg) {
    //printf("sendMSG_in(s(%d), type(%d), orig(%d), dest(%d), sequ(%d), length(%d), msg(%s))\n", s, type, orig, dest, sequ, length, msg);
    Mensagem m;
    m.type = type;
    m.orig = orig;
    m.dest = dest;
    m.sequ = sequ;
    m.length = length;
    memcpy(m.msg, msg, length);
    
    if(send(s, &m, 10 + length, 0) < 0){
        perror("error: sendMSG");
        close(s);
        exit(1);
    }
    //printf("sendMSG_out()\n");
}

void recvData(int s, char* buff) {
    Mensagem* msg = (Mensagem*) buff;

    //printf("recvData_in(s(%d), buff(%p))\n", s, buff);

    //Recebimento do cabeçalho
    if(recv(s, buff, 10, 0) < 0)
        perror("error: recv1");

    if(msg->type == MSG) {
        if(recv(s, &buff[10], msg->length, 0) < 0)
            perror("error: recv2");
    }

    //printf("recvData_out()\n");
}

<<<<<<< HEAD:mensagem.c
void wait(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ) {
    Mensagem msg;
=======
void wait(uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ) {
>>>>>>> master:src/mensagem.c
    while(1) {
        recvData(s, (char*) &msg);
        if(msg.type == type && msg.orig == orig && msg.dest == dest && msg.sequ == sequ)
            return;
    }
}