#ifndef CABECALHO_H
#define CABECALHO_H

#include <sys/types.h>

#define OK 1
#define ERRO 2
#define OI 3
#define FLW 4
#define MSG 5
#define CREQ 6
#define CLIST 7

typedef struct {
    int16_t type;
    int16_t orig;
    int16_t dest;
    int16_t sequ;
} Cabecalho;

typedef struct {
    int16_t length;
    char msg[INT16_MAX];
} Mensagem;


#endif