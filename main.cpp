#include <iostream>
#include "NetworkHeaders.h"
#include "Peer.h"

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
#endif
    Peer client;
    std::string address;
    int port;
    std::cout << "Ingresa la direccion IP:" ;
    std::cin >> address;
    std::cout << "Ingresa el Port: ";
    std::cin >> port;
    client.FindPeer(const_cast<char *>(address.c_str()),port);


#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}