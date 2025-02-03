//
// Created by Cuent on 1/30/2025.
//

#ifndef UDPSOCKET_H
#define UDPSOCKET_H


#include "NetworkHeaders.h"
class UDPSocket {
public:


    bool Initialize();
    bool Bind();

    int SendTo(char *buffer, int bufferLen, sockaddr *to, int toLen);

    int ReceiveFrom(char *buffer, int bufferLen, sockaddr *from, socklen_t *fromLen);
    bool GetIsSocketValid();

    ~UDPSocket();

private:
    SOCKET m_socket;

};
#endif //UDPSOCKET_H
