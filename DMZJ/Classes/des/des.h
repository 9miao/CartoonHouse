#ifndef _MYDES_H
#define _MYDES_H
#include "base64.h"
namespace des
{
    
    int DESCrypto(char *src, char *des, int len, char *key, char *iv, int decrypt, int tostr);
}


#endif
