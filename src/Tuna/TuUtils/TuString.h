#ifndef TUSTRING_H
#define TUSTRING_H

#include "TuDefines.h"
#include <stdint.h>

/**
 * @param dest the len of this string need
 * @param minStrLen if the 0 the function is not, and will be copying until fount the string termination symbol
 */
void tuSafeStrCopy(char* dest, char* src, uint64_t minStrLen);

/**
 * @return returns the new string allocated with malloc, need to call free after using
 */
char* tuStringSum(char* first, char* second, uint64_t firstLen, uint64_t secondLen);
TuBool tuContains(char* src, char* what, uint64_t srcLen, uint64_t whatLen);

#endif // TUSTRING_H
