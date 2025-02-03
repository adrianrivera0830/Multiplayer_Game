//
// Created by Cuent on 1/30/2025.
//

#include "UDPSocket.h"
#include <iostream>

bool UDPSocket::Initialize() {
    // Configuración del socket


    // Creación del socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (!ISVALIDSOCKET(m_socket)) {
        std::cerr << "Error al crear el socket: " << GETSOCKETERRNO() << std::endl;
        return false;
    }

    if (!setSocketNonBlocking(m_socket)) {
        return false;
    }


    return true;
}

bool UDPSocket::Bind() {
    sockaddr_in socketConfig;
    memset(&socketConfig, 0, sizeof(socketConfig)); // Inicializa en 0
    socketConfig.sin_family = AF_INET;
    socketConfig.sin_addr.s_addr = INADDR_ANY;
    socketConfig.sin_port = htons(0); // Puerto aleatorio

    if (bind(m_socket, (struct sockaddr *) &socketConfig, sizeof(socketConfig)) == -1) {
        std::cerr << "Error en bind(): " << GETSOCKETERRNO() << std::endl;
        return false;
    }
    // Obtener la dirección y puerto asignados
    sockaddr_in boundAddress;
    socklen_t boundAddressLen = sizeof(boundAddress);
    if (getsockname(m_socket, (struct sockaddr*)&boundAddress, &boundAddressLen) == -1)
    {
        std::cerr << "Error en getsockname(): " << GETSOCKETERRNO() << std::endl;
        return false;
    }


    // Obtener el nombre del host
    char localHostname[NI_MAXHOST];
    if (gethostname(localHostname, sizeof(localHostname)) == -1)
    {
        std::cerr << "Error en gethostname(): " << GETSOCKETERRNO() << std::endl;
        return false;
    }

    std::cout << "Nombre del host: " << localHostname << "\n";

    // Obtener direcciones IP disponibles
    struct addrinfo hints, * addrInfoList;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE; // Correcto para obtener direcciones IP
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(localHostname, nullptr, &hints, &addrInfoList) != 0)
    {
        std::cerr << "Error en getaddrinfo(): " << GETSOCKETERRNO() << std::endl;
        return false;
    }

    std::cout << "Direcciones IP disponibles:\n";
    for (struct addrinfo* currentAddr = addrInfoList; currentAddr != nullptr; currentAddr = currentAddr->ai_next)
    {
        sockaddr_in* ipAddress = reinterpret_cast<sockaddr_in*>(currentAddr->ai_addr);
        char ipStr[INET_ADDRSTRLEN];

        if (inet_ntop(AF_INET, &ipAddress->sin_addr, ipStr, sizeof(ipStr)) == nullptr)
        {
            std::cerr << "Error al convertir la dirección IP\n";
            continue; // Saltar esta dirección si hay un problema
        }

        std::cout << ipStr << "\n";
    }

    std::cout << "Puerto privado: " << ntohs(boundAddress.sin_port) << "\n\n";


    freeaddrinfo(addrInfoList); // Liberar memoria de `getaddrinfo`

    return true;

}

int UDPSocket::SendTo(char *buffer, int bufferLen, sockaddr *to, socklen_t toLen) {
    int bytes_sent = sendto(m_socket, buffer, bufferLen, 0, to, toLen);

    return bytes_sent;
}

int UDPSocket::ReceiveFrom(char *buffer, int bufferLen, sockaddr *from, socklen_t *fromLen) {
    int bytes_received = recvfrom(m_socket,  buffer, bufferLen, 0, from, fromLen);


    if (bytes_received > 0 && bytes_received < bufferLen) {
        buffer[bytes_received] = '\0';
    }

    return bytes_received;
}

bool UDPSocket::GetIsSocketValid() {
    return ISVALIDSOCKET(m_socket);
}



UDPSocket::~UDPSocket()
{
    CLOSESOCKET(m_socket);
#ifdef _WIN32
    WSACleanup();
#endif
}
