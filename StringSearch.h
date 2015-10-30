#ifndef __STRING_SEARCH_H__
#define __STRING_SEARCH_H__

#include <Windows.h>
#include <mbstring.h>

bool IsSimplifiedCH();

// DWORD IsAscii(byte* pbMem,bool& bAscii);
// bool  IsByteAscii(byte byAsc);
// bool IsAsciiEx(byte* pbMem, DWORD& dwLen);
// bool IsAsciiRepeat(byte* pbMem, DWORD dwLength, DWORD& dwRepeatLen);

bool IsSimplifiedCH(WORD wChar);
bool IsByteAscii(byte byAsc);
DWORD GetUnicodeAsciiLen(const BYTE* pMem, DWORD dwBufLen);
DWORD IsAscii( BYTE* pbMem, BOOL& bAscii );
// 判断是否是字符,包括中文字符
bool IsAsciiOrUnicodeString(byte* pbMem, DWORD& dwLen, BOOL& bIsUnicode, DWORD dwMaxCount);

/*
*	@brief
*       判断是否是字符串或unicode
*	@param
*       pbMem       待判断内存
*       dwMemLen    pbMem的大小
*       dwLen       如果是字符串或unicode,则存放长度(in byte)
*       bIsUnicode  存放是否是unicode
*	@ret
*       是否是unicode或ascii
*	@comment
*       
*/
BOOL IsAsciiOrUnicodeStringEx(byte* pbMem,
                              DWORD dwMemLen, 
                              DWORD& dwLen, 
                              BOOL& bIsUnicode);

#define MAX_CHECK_STRING_LEN    4096

#endif
