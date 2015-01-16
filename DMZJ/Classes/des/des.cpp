#include <string.h>
#include <stdlib.h>
#include "av_des.h"
#include "av_md5.h"
#include "des.h"

#define CHECK_NULL_RET(bCondition) if (!bCondition) goto Exit0
#define BUFSIZE 1024
#define MD5LEN  16

namespace des
{

int MD5Sum(unsigned char *src, unsigned char *des, size_t len)
{
	uint8_t res[20] = {0};
	struct AVMD5 *ctx = NULL;
	ctx = av_md5_alloc();
	av_md5_init(ctx);
	if (len > 0)
	{
		av_md5_update(ctx, src, len);
	}
	av_md5_final(ctx, res);
	free(ctx);
	for (int i = 0; i < 16; i++)
	{
		des[i*2] = "0123456789abcdef"[(res[i]&0xF0)>>4];
		des[i*2+1] = "0123456789abcdef"[res[i]&0x0F];
	}
	des[32] = 0;
	return 0;
}

int DESCrypto(char *src, char *des, int len, char *key, char *iv, int decrypt, int tostr)
{
	struct AVDES d;
	int retLen = 0;
	char kmd5[33] = "";
	//char ivmd5[33] = "";
	char *tsrc = (char*)malloc((len + 20) * sizeof(char));
	memset(tsrc, 0, ((len + 7) * sizeof(char)));
	memcpy(tsrc, src, len);
	MD5Sum((uint8_t*)key, (uint8_t*)kmd5, strlen(key));
	//MD5Sum((uint8_t*)iv, (uint8_t*)ivmd5, strlen(iv));
	kmd5[8] = 0;
	//ivmd5[8] = 0;
	av_des_init(&d, (uint8_t*)kmd5, 64, 0);
	int cnt = 8 - len % 8;
	int i = 0;
	for (i = len; i < len + cnt; i++)
	{
		tsrc[i] = cnt;
	}
	cnt = (len + 8) * sizeof(char) / 8;
	av_des_crypt(&d, (uint8_t*)tsrc, (uint8_t*)tsrc, cnt , (uint8_t*)iv, decrypt);
	if (tostr)
	{
		for (i = 0; i < cnt * 8; i ++)
		{
			des[i*2] = "0123456789abcdef"[(tsrc[i]&0xF0)>>4];
			des[i*2+1] = "0123456789abcdef"[tsrc[i]&0x0F];
		}
		des[i * 2] = 0;
		retLen = cnt * 8 * 2;
	}
	else
	{
		memcpy(des, tsrc, cnt * 8);
		retLen = cnt * 8;
	}
	free(tsrc);
	return retLen;
}
    
}
