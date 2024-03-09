//
// Created by kgaft on 3/9/24.
//
#include <TuDrivers/TuSockets.h>
#include <stdio.h>
#include <string.h>

int main(){
    TuSocketCreateInfo servInfo;
    servInfo.ipAddress = 0;
    servInfo.socketType = TU_SOCKET_IPV4;
    servInfo.connectionType = TU_SOCKET_TCP;
    servInfo.socketPort = 9032;

    TuSocketServer server;
    if(tuCreateServer(&servInfo, &server)!=TU_SUCCESS){
        perror("Failed");
        return -1;
    }
    TuSocketIncomingClient client;
    char buffer[16196];

    TuSocketCreateInfo secondInfo;
    secondInfo.ipAddress = "192.168.1.0";
    secondInfo.socketType = TU_SOCKET_IPV4;
    secondInfo.connectionType = TU_SOCKET_TCP;
    secondInfo.socketPort = 0;
    TuSocketClient client1;
    tuCreateClient(&secondInfo, &client1);
    tuConnectClient(client1, &secondInfo);
    perror("Test");
    while(1){
        tuListenServer(&server);
        if(tuAcceptClient(&server, &client)!=TU_SUCCESS){
            perror("Failed to accept");
        }
        TuSize read;
        tuReadClient(&client, buffer, 16196, &read);
        if(read>0){
            tuSendDataToServer(client1, buffer, read);
            tuReadServer(client1, buffer, 16196, &read);
            if(read>0){
                tuSendDataToClient(&client, buffer, read);
            }
        }
    }
    return 0;
}