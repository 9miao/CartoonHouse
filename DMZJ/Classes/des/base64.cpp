#include "base64.h"

int Base64Encode(unsigned char* pDes, const unsigned char* pSource, int& size)
{
    //编码表
    const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
    unsigned char Tmp[4]={0};
    int LineLength=0;
	unsigned char *pOut = pDes;
    for(int i = 0; i < (int)(size / 3); i++)
    {
        Tmp[1] = *pSource++;
        Tmp[2] = *pSource++;
        Tmp[3] = *pSource++;
        *pOut = EncodeTable[Tmp[1] >> 2]; pOut ++;
        *pOut = EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F]; pOut ++;
        *pOut = EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F]; pOut ++;
        *pOut = EncodeTable[Tmp[3] & 0x3F]; pOut ++;
    }
    //对剩余数据进行编码
    int Mod=size % 3;
    if(Mod==1)
    {
        Tmp[1] = *pSource++;
        *pOut = EncodeTable[(Tmp[1] & 0xFC) >> 2];		pOut ++;
        *pOut = EncodeTable[((Tmp[1] & 0x03) << 4)];	pOut ++;
        *pOut = '='; pOut ++;
        *pOut = '='; pOut ++;
    }
    else if(Mod==2)
    {
        Tmp[1] = *pSource++;
        Tmp[2] = *pSource++;
        *pOut = EncodeTable[(Tmp[1] & 0xFC) >> 2]; pOut ++;
        *pOut = EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)]; pOut ++;
        *pOut = EncodeTable[((Tmp[2] & 0x0F) << 2)]; pOut ++;
        *pOut = '='; pOut ++;
    }
	*pOut = 0;
	size = pOut - pDes;
    return 0;
}

int Base64Codec(unsigned char* pDes, const unsigned char* pSource, int& size)
{
	if (size % 4 != 0)
	{}
	//	throw std::runtime_error("invalid base64 string");
    //解码表
    const char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };

	int nValue;
    int i= 0;
	int inBytes = size;
	unsigned char *pOut = pDes;
	size = 0;
    while (i < inBytes)
    {
        if (*pSource != '\r' && *pSource!='\n')
        {
			if (*pSource >= 123)
			{}
				//throw std::runtime_error("invalid base64 string");
            nValue = DecodeTable[*pSource++] << 18;
            nValue += DecodeTable[*pSource++] << 12;
            *pOut = (nValue & 0x00FF0000) >> 16;	pOut ++;
            size++;
            if (*pSource != '=')
            {
                nValue += DecodeTable[*pSource++] << 6;
                *pOut = (nValue & 0x0000FF00) >> 8;	pOut ++;
                size++;
                if (*pSource != '=')
                {
                    nValue += DecodeTable[*pSource++];
                    *pOut = nValue & 0x000000FF;	pOut ++;
                    size++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            pSource++;
            i++;
        }
     }
	*pOut = 0;
	return 0;
}
