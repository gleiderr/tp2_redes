#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <stdint.h>
#include <math.h>

#define SERVER_ID 65535

/* Tipos de mensagem definidas no enunciado. */
#define OK 1
#define ERRO 2
#define OI 3
#define FLW 4
#define MSG 5
#define CREQ 6
#define CLIST 7

/* Tipo de mensagem para comunição entre os hosts. */
typedef struct {
    uint16_t type;
    uint16_t orig;
    uint16_t dest;
    uint16_t sequ;
    uint16_t length;
    char msg[UINT16_MAX];
} Mensagem;

/*
 * Formata a mensagem e a envia ao destinatario certo. 
 */
void sendMSG(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ, uint16_t length, char* msg);

/**
 * Recebe a Mensagem no seu socket e valida se a mesma chegou corretamente. 
 */ 
void recvData(int s, char* buff);

/*
 * Espera pelo recebimento completo, para garatir recebimento da mensagem. 
 */
void wait(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ);

#endif