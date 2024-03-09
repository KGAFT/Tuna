#ifndef TUSOCKETS_H
#define TUSOCKETS_H

#include <stdint.h>
#include <TuDefines.h>

typedef enum TuSocketType {
    TU_SOCKET_LOCAL = 0,
    TU_SOCKET_IPV4 = 1,
    TU_SOCKET_IPV6 = 2
} TuSocketType;

typedef enum TuSocketConnectionType {
    TU_SOCKET_UDP = 0,
    TU_SOCKET_TCP = 1
} TuSocketConnectionType;

typedef struct TuSocketCreateInfo {
    TuSocketType socketType;
    TuSocketConnectionType connectionType;
    uint16_t socketPort;
    const char *ipAddress;

} TuSocketCreateInfo;
typedef struct TuSocketIncomingClient {
    uint64_t id;
    char ipAddress[15];

} TuSocketIncomingClient;

typedef struct TuSocketServer {
    uint64_t id;
    TuSocketConnectionType connectionType;
    TuSocketType socketType;
} TuSocketServer;
typedef uint32_t TuSocketClient;

TuResult tuCreateServer(TuSocketCreateInfo *pCreateInfo, TuSocketServer *pOutput);

TuResult tuCreateClient(TuSocketCreateInfo *pCreateInfo, TuSocketClient *pOutput);

TuResult tuConnectClient(TuSocketClient client, TuSocketCreateInfo *createInfo);

void tuDisconnect(TuSocketClient client);

void tuDisconnectServer(TuSocketServer *pServer);

void tuDisconnectIncomingClient(TuSocketIncomingClient* incomingClient);

void tuListenServer(TuSocketServer *server);

TuResult tuAcceptClient(TuSocketServer *server, TuSocketIncomingClient *pOutput);

void tuReadClient(TuSocketIncomingClient *pClient, void *buffer, TuSize bufferSize, TuSize *pReadSizeOutput);

void tuReadServer(TuSocketClient client, void *buffer, TuSize bufferSize, TuSize *pReadSizeOutput);

void tuSendDataToServer(TuSocketClient client, void *data, TuSize dataSize);

void tuSendDataToClient(TuSocketIncomingClient *target, void *data, TuSize dataSize);


#endif // TUSOCKETS_H
