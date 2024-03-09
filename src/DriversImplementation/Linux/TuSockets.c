//
// Created by kgaft on 3/9/24.
//
#include <TuDrivers/TuSockets.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


sa_family_t socketTypeToDomain(TuSocketType connectionType);

int connectionTypeToDef(TuSocketConnectionType type);

TuResult tuCreateServer(TuSocketCreateInfo *pCreateInfo, TuSocketServer *pOutput) {
    int serverFd, opt = 1;
    struct sockaddr_in address;
    if ((serverFd = socket(socketTypeToDomain(pCreateInfo->socketType),
                           connectionTypeToDef(pCreateInfo->connectionType), 0)) < 0) {
        return TU_FAILED_TO_CREATE_SOCKET;
    }
    if (setsockopt(serverFd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        return TU_FAILED_TO_RESERVE_SOCKET;
    }
    address.sin_family = socketTypeToDomain(pCreateInfo->socketType);
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(pCreateInfo->socketPort);
    if (bind(serverFd, (struct sockaddr *) &address,
             sizeof(address))
        < 0) {
        return TU_FAILED_TO_BIND_ADDRESS_INFO_TO_SOCKET;
    }
    pOutput->id = serverFd;
    pOutput->connectionType = pCreateInfo->connectionType;
    pOutput->socketType = pCreateInfo->socketType;
    return TU_SUCCESS;
}

TuResult tuCreateClient(TuSocketCreateInfo *pCreateInfo, TuSocketClient *pOutput) {
    int clientFd;
    if ((clientFd = socket(socketTypeToDomain(pCreateInfo->socketType),
                           connectionTypeToDef(pCreateInfo->connectionType), 0)) < 0) {
        return TU_FAILED_TO_CREATE_SOCKET;
    }
    *pOutput = clientFd;
    return TU_SUCCESS;
}

TuResult tuConnectClient(TuSocketClient client, TuSocketCreateInfo *pCreateInfo) {
    struct sockaddr_in servAddr;
    int status;
    servAddr.sin_family = socketTypeToDomain(pCreateInfo->socketType);
    servAddr.sin_port = htons(pCreateInfo->socketPort);
    if (inet_pton(socketTypeToDomain(pCreateInfo->socketType), pCreateInfo->ipAddress, &servAddr.sin_addr)
        <= 0) {
        return TU_FAILED_TO_CONVERT_SOCKET_ADDRESS;
    }
    if ((status
                 = connect(client, (struct sockaddr *) &servAddr,
                           sizeof(servAddr)))
        < 0) {
        return TU_FAILED_TO_CONNECT_SOCKET;
    }
    return TU_SUCCESS;
}

void tuDisconnect(TuSocketClient client) {
    close(client);
}

void tuDisconnectServer(TuSocketServer *pServer) {
    close(pServer->id);
}

void tuDisconnectIncomingClient(TuSocketIncomingClient* incomingClient){
    close(incomingClient->id);
}

void tuListenServer(TuSocketServer *server) {
    listen(server->id, INT32_MAX);
}

TuResult tuAcceptClient(TuSocketServer *server, TuSocketIncomingClient *pOutput) {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = 0;
    if ((clientFd
                 = accept(server->id, (struct sockaddr *) &clientAddr,
                          &addrLen))
        < 0) {
        return TU_FAILED_TO_ACCEPT;
    }
    pOutput->id = (uint64_t) clientFd;
    inet_ntop(socketTypeToDomain(server->socketType), &clientAddr.sin_addr, pOutput->ipAddress, 15);
    return TU_SUCCESS;
}

void tuSendDataToServer(TuSocketClient client, void *data, TuSize dataSize) {
    send(client, data, dataSize, 0);
}

void tuSendDataToClient(TuSocketIncomingClient *target, void *data, TuSize dataSize) {
    send(target->id, data, dataSize, 0);
}

void tuReadClient(TuSocketIncomingClient *pClient, void *buffer, TuSize bufferSize, TuSize *pReadSizeOutput) {
    *pReadSizeOutput = read(pClient->id, buffer, bufferSize);
}

void tuReadServer(TuSocketClient client, void *buffer, TuSize bufferSize, TuSize *pReadSizeOutput) {
    *pReadSizeOutput = read(client, buffer, bufferSize);
}


sa_family_t socketTypeToDomain(TuSocketType connectionType) {
    switch (connectionType) {
        case TU_SOCKET_LOCAL:
            return AF_LOCAL;
        case TU_SOCKET_IPV4:
            return AF_INET;
        case TU_SOCKET_IPV6:
            return AF_INET6;
    }
}

int connectionTypeToDef(TuSocketConnectionType type) {
    switch (type) {
        case TU_SOCKET_TCP:
            return SOCK_STREAM;
        case TU_SOCKET_UDP:
            return SOCK_DGRAM;
    }
}