//
// Created by Cuent on 1/30/2025.
//

#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "UDPSocket.h";

class UDPClient {
public:
    UDPClient() {
        socket.Initialize();
    }

    void CommunicationLoop();

private:
    UDPSocket socket;
};



#endif //UDPCLIENT_H
