#ifndef _CODEC_H
#define _CODEC_H
#pragma once

#include <iostream>

int Base64Encode(unsigned char* pDes, const unsigned char* pSource, int& size);
int Base64Codec(unsigned char* pDes, const unsigned char* pSource, int& size);

#endif
