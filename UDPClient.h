//
// Created by Cuent on 1/30/2025.
//

#ifndef UDPCLIENT_H
#define UDPCLIENT_H


#include "UDPSocket.h";

class UDPClient {
public:
    UDPClient() {
        if (!socket.Initialize()) return;
        if (!socket.Bind()) return;
        GetPublicIP();


    }

    void GetPublicIP();
    void FindPeer(char *address, int port);
    void CommunicationLoop();
    UDPSocket socket;

private:
};


#endif //UDPCLIENT_H
