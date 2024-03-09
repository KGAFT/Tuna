#include "TuDrivers/TunInterface.h"
#include "TuUtils/TuString.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    TunInterfaceConfiguration config;

    config.preferredDeviceName = "tun";
    config.keepProbing = TU_TRUE;
    tuSafeStrCopy(config.interfaceName, "tuna", 4);
    TunInterface interface;
    if(tuConnectToInterface(&config, &interface)!=TU_SUCCESS){
        perror("Failed: ");
        return 1;
    }
    char buffer[16196];
    while(1){
        TuSize read = 0;
        if(tuReadFromInterface(interface, buffer, 16196, &read)==TU_SUCCESS){
            for(TuSize i = 0; i<read; i++){
                printf("%c", buffer[i]);
            }
            if(read){
                printf("\n");
            }
        }
    }
	return 0;
}
