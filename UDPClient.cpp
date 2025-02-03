#include <iostream>
#include "UDPClient.h"
#include <cstdlib>
#include <string>
#define STUN_SERVER "stun.l.google.com"
#define STUN_PORT 19302

void UDPClient::GetPublicIP() {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(STUN_PORT);
    inet_pton(AF_INET, "74.125.197.127", &server_addr.sin_addr);

    unsigned char stun_request[20] = {0x00, 0x01, 0x00, 0x00};
    srand((unsigned int) time(NULL));
    for (int i = 4; i < 20; i++) {
        stun_request[i] = rand() % 256;
    }

    int bytes = socket.SendTo((char *) stun_request, sizeof(stun_request),(struct sockaddr *) &server_addr, sizeof(server_addr));

    //
    unsigned char response[512];
    struct sockaddr_in from;
    socklen_t from_len = sizeof(from);
    int received;
    while (true) {
        received = socket.ReceiveFrom((char *) response, sizeof(response),(struct sockaddr *) &from, &from_len);
        if (received > 0) {
            break;
        }
    }

    int mapped_port = (response[24] << 8) | response[25];
    mapped_port ^= 0x2112; // Aplicar XOR con la máscara estándar de STUN (0x2112)

    unsigned char *mapped_addr = response + 26;
    std::cout << " IP Publica: " << (int) mapped_addr[0] << "." << (int) mapped_addr[1] << "."
            << (int) mapped_addr[2] << "." << (int) mapped_addr[3] << std::endl;
    std::cout << " Puerto Publico: " << mapped_port << std::endl;
}

void UDPClient::FindPeer(char *address,int port) {
    char *buffer = "hola mundo";

    sockaddr_in peerAddress;
    memset(&peerAddress, 0, sizeof(peerAddress));
    peerAddress.sin_family = AF_INET;
    peerAddress.sin_port = htons(port);
    peerAddress.sin_addr.s_addr = inet_addr(address);

    socklen_t peerLength = sizeof(peerAddress);

    while (true) {
        int bytes_sent = socket.SendTo(buffer,strlen(buffer),(sockaddr *)&peerAddress,peerLength);

        int bytes_received = socket.ReceiveFrom((char *)buffer,strlen(buffer),(sockaddr *)&peerAddress,&peerLength);

        if (bytes_received > 0) {
            break;
        }
        std::cout << bytes_received << std::endl;

    }




}

void UDPClient::CommunicationLoop() {
}
