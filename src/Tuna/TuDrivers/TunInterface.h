#ifndef TUNA_H
#define TUNA_H

#include <stdint.h>
#include <TuDefines.h>

typedef uint32_t TunInterface;
#define MAX_LEN_INTERFACE_NAME 14

/**
 * @param interfaceName is specifying the name of virtual device, that will be available in the operating system
 * @param preferredDeviceName specifying the preferred base device interface name in the system, if the interface with this name will not be found, will be opened the first available interface
 * @param keepProbingInterfaces this parameter is specified to keep trying to open another interfaces name if the specified in preferredDeviceName will not be found or will not be available
 */
typedef struct TunInterfaceConfiguration{
    char interfaceName[MAX_LEN_INTERFACE_NAME];
    char* preferredDeviceName;
    TuBool keepProbing;

} TunInterfaceConfiguration;


TuResult tuConnectToInterface(TunInterfaceConfiguration* pConfig, TunInterface* pOutput);
TuResult tuReadFromInterface(TunInterface source, void* buffer, TuSize maxReadSize, TuSize* pReadSizeOutput);
TuResult tuWriteToInterface(TunInterface target, void* buffer, TuSize writeSize);

#endif // TUNA_H
