#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

/**
 *
 */

/**
 *
 */
void sendMSG(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ, uint16_t length, char* msg);

/**
 *
 */
void recvData(int s, char* buff);

#endif