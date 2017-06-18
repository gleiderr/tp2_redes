#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <stdint.h>
#include <math.h>

#define SERVER_ID 65535

#define OK 1
#define ERRO 2
#define OI 3
#define FLW 4
#define MSG 5
#define CREQ 6
#define CLIST 7

typedef struct {
    uint16_t type;
    uint16_t orig;
    uint16_t dest;
    uint16_t sequ;
    uint16_t length;
    char msg[];
} Mensagem;

#endif