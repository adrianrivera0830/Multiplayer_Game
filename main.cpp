#include <iostream>
#include "NetworkHeaders.h"
#include "UDPClient.h"

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
#endif
    UDPClient client;

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}