//
// Created by Cuent on 1/30/2025.
//

#include "UDPSocket.h"
#include <iostream>

void UDPSocket::Initialize()
{
    // Configuración del socket
    sockaddr_in socketConfig;
    memset(&socketConfig, 0, sizeof(socketConfig)); // Inicializa en 0
    socketConfig.sin_family = AF_INET;
    socketConfig.sin_addr.s_addr = INADDR_ANY;
    socketConfig.sin_port = htons(0); // Puerto aleatorio

    // Creación del socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (!ISVALIDSOCKET(m_socket))
    {
        std::cerr << "Error al crear el socket: " << GETSOCKETERRNO() << std::endl;
        return;
    }

    if (bind(m_socket, (struct sockaddr*)&socketConfig, sizeof(socketConfig)) == -1)
    {
        std::cerr << "Error en bind(): " << GETSOCKETERRNO() << std::endl;
        return;
    }

    // Obtener la dirección y puerto asignados
    sockaddr_in boundAddress;
    socklen_t boundAddressLen = sizeof(boundAddress);
    if (getsockname(m_socket, (struct sockaddr*)&boundAddress, &boundAddressLen) == -1)
    {
        std::cerr << "Error en getsockname(): " << GETSOCKETERRNO() << std::endl;
        return;
    }

    std::cout << "Puerto asignado: " << ntohs(boundAddress.sin_port) << "\n";

    // Obtener el nombre del host
    char localHostname[NI_MAXHOST];
    if (gethostname(localHostname, sizeof(localHostname)) == -1)
    {
        std::cerr << "Error en gethostname(): " << GETSOCKETERRNO() << std::endl;
        return;
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
        return;
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

        std::cout << "- " << ipStr << "\n";
    }

    freeaddrinfo(addrInfoList); // Liberar memoria de `getaddrinfo`
}

bool UDPSocket::SendTo(void* buffer, int bufferLen, sockaddr* to, int toLen)
{
    int bytes_sent = sendto(m_socket, (char*)buffer, bufferLen, 0, to, toLen);
    if (bytes_sent == SOCKET_ERROR)
    {
        std::cerr << "Error en SendTo: " << GETSOCKETERRNO() << std::endl;
        return false;
    }
    return true;
}

bool UDPSocket::ReceiveFrom(void* buffer, int bufferLen, sockaddr* from, socklen_t* fromLen)
{
    int bytes_received = recvfrom(m_socket, (char*)buffer, bufferLen, 0, from, fromLen);
    if (bytes_received == SOCKET_ERROR)
    {
        std::cerr << "Error en ReceiveFrom: " << GETSOCKETERRNO() << std::endl;
        return false;
    }

    if (bytes_received > 0 && bytes_received < bufferLen) {
        ((char*)buffer)[bytes_received] = '\0';
    }

    return true;
}

bool UDPSocket::GetIsSocketValid()
{
    return ISVALIDSOCKET(m_socket);
}

// UDPSocket::~UDPSocket()
// {
//     CLOSESOCKET(m_socket);
// #ifdef _WIN32
//     WSACleanup();
// #endif
// }