#include <TuDrivers/TunInterface.h>
#include <TuUtils/TuString.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>

#define TUN_DIR "/dev/net/"
#define TUN_DIR_LEN 10

TuResult tuConnectToInterface(TunInterfaceConfiguration* pConfig, TunInterface* pOutput){
    char interfaceName[IFNAMSIZ];
    tuSafeStrCopy(interfaceName, pConfig->interfaceName, MAX_LEN_INTERFACE_NAME);
    char* deviceName = tuStringSum(TUN_DIR, pConfig->preferredDeviceName, TUN_DIR_LEN, 0);
    int fd = open(deviceName, O_RDWR);
    free(deviceName);
    if(fd<0 && !pConfig->keepProbing){
        return TU_INTERFACE_NOT_FOUND;
    }
    if(fd<0){
        DIR *d;
        struct dirent *dir;
        d = opendir(TUN_DIR);
        while(d && ((dir=readdir(d))!=NULL) && (fd<0)){
            if(tuContains(dir->d_name, "tun", 0, 3)){
                deviceName = tuStringSum(TUN_DIR, dir->d_name, TUN_DIR_LEN, 0);
                fd = open(deviceName, O_RDWR);
                free(deviceName);
            }
        }
        if(d<0){
            return TU_FAILED_TO_OPEN_INTERFACE;
        }
    }
    struct ifreq deviceSetup;
    memset((void*)&deviceSetup, 0, sizeof(deviceSetup));
    deviceSetup.ifr_flags = IFF_TUN;
    int err = 0;
    if((err = ioctl(fd, TUNSETIFF, (void*)&deviceSetup))<0){
        return TU_FAILED_TO_SETUP_INTERFACE;
    }
    tuSafeStrCopy(interfaceName, deviceSetup.ifr_name, IFNAMSIZ);
    *pOutput = fd;

    return TU_SUCCESS;
}

TuResult tuReadFromInterface(TunInterface source, void* buffer, TuSize maxReadSize, TuSize* pReadSizeOutput){
    int tmpRes = read(source, buffer, maxReadSize);
    if(tmpRes<0){
        return TU_FAILED;
    }
    *pReadSizeOutput = tmpRes;
    return TU_SUCCESS;
}
TuResult tuWriteToInterface(TunInterface target, void* buffer, TuSize writeSize){
    write(target, buffer, writeSize);
    return TU_SUCCESS;
}
