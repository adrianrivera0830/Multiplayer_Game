//
// Created by Cuent on 1/30/2025.
//

#ifndef UDPSOCKET_H
#define UDPSOCKET_H


#include "NetworkHeaders.h"
class UDPSocket {
public:
    UDPSocket()
    {
        Initialize();
    }
    void Initialize();
    bool SendTo(void* buffer, int bufferLen, sockaddr* to, int toLen);
    bool ReceiveFrom(void* buffer, int bufferLen, sockaddr* from, socklen_t* fromLen);
    bool GetIsSocketValid();

private:
    SOCKET m_socket;
};
#endif //UDPSOCKET_H
