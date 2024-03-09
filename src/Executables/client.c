//
// Created by kgaft on 3/9/24.
//
#include <TuDrivers/TuSockets.h>
#include <stdio.h>
#include <string.h>
#include "TuDrivers/TunInterface.h"
#include "TuUtils/TuString.h"

int main(){
    TunInterfaceConfiguration config;

    config.preferredDeviceName = "tun";
    config.keepProbing = TU_TRUE;
    tuSafeStrCopy(config.interfaceName, "tuna", 4);
    TunInterface interface;
    if(tuConnectToInterface(&config, &interface)!=TU_SUCCESS){
        perror("Failed: ");
        return 1;
    }

    TuSocketCreateInfo createInfo;
    createInfo.ipAddress = "127.0.0.1";
    createInfo.connectionType = TU_SOCKET_TCP;
    createInfo.socketType = TU_SOCKET_IPV4;
    createInfo.socketPort = 9032;
    TuSocketClient client;
    if(tuCreateClient(&createInfo, &client)!=TU_SUCCESS){
        perror("Failed: ");
    }
    while(tuConnectClient(client, &createInfo)!=TU_SUCCESS){

    }
    char buffer[16196];
    while(1){
        TuSize read = 0;
        if(tuReadFromInterface(interface, buffer, 16196, &read)==TU_SUCCESS){
            tuSendDataToServer(client, buffer, read);
        }
        tuReadServer(client, buffer, 16196, &read);
        if(read>0){
            tuWriteToInterface(interface, buffer, read);
        }
    }


    return 0;
}