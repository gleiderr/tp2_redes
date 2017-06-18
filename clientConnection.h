#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

int openClient(char const* addr);
void sendOI(int s, uint16_t sequ);


#endif