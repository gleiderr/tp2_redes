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
#include "serverConnection.h"

void sendMSG(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ, uint16_t length, char* msg) {
    //printf("sendMSG(s(%d), type(%d), orig(%d), dest(%d), sequ(%d), length(%d))\n", s, type, orig, dest, sequ, length);
    Mensagem m;
    m.type = type;
    m.orig = orig;
    m.dest = dest;
    m.sequ = sequ;
    m.length = length;
    memcpy(m.msg, msg, length);
    
    if(send(s, &m, 10 + m.length, 0) < 0){
        perror("error: sendMSG");

        close(s);
        exit(1);
    }
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

