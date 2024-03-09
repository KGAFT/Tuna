#include "TuString.h"
#include <string.h>
#include <stdlib.h>

void tuSafeStrCopy(char* dest, char* src, uint64_t minStrLen){
    if(minStrLen==0){
        uint64_t i = 0;
        while(src[i]!='\0'){
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    } else {
        uint64_t i = 0;
        for(i = 0; (i<minStrLen && src[i]!='\0'); i++){
            dest[i] = src[i];
        }
        if(i<minStrLen && src[i]=='\0'){
            dest[i] = '\0';
        } else {
            dest[minStrLen] = '\0';
        }
    }
}

char* tuStringSum(char* first, char* second, uint64_t firstLen, uint64_t secondLen){
    if(!firstLen){
        firstLen = strlen(first);
    }
    if(!secondLen){
        secondLen = strlen(second);
    }
    uint64_t stringLen = firstLen+secondLen+1;
    char* result = calloc(sizeof(char), stringLen+1);
    uint64_t i = 0;
    while(first[i]!='\0'){
        result[i] = first[i];
        i++;
    }
    uint64_t ii = 0;
    while(second[ii]!='\0'){
        result[i] = second[ii];
        ii++;
        i++;
    }
    result[stringLen] = '\0';
    return result;
}

TuBool tuContains(char* src, char* what, uint64_t srcLen, uint64_t whatLen){
    if(!srcLen){
        srcLen = strlen(src);
    }
    if(!whatLen){
        whatLen = strlen(what);
    }
    uint64_t i = 0;

    while(i+whatLen<=srcLen){
        TuBool found = TU_TRUE;
        for(uint64_t ii = i; ii<i+whatLen; ii++){
            if(src[ii]!=what[ii-i]){
                found = TU_FALSE;
                break;
            }
        }
        if(found){
            return found;
        }
        i++;
    }
    return TU_FALSE;
}

