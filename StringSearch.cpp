#include "StringSearch.h"

bool IsSimplifiedCH(WORD wChar)
{
	byte bHigh = 0;
	byte bLow = 0;

	bHigh = wChar & 0xff;
	bLow = wChar /  0x100;

	if ((bHigh >= 0xa1 && bHigh <= 0xf7) && (bLow >= 0xa1 && bLow <= 0xfe))
	{
		return true;
	}

	return false;
}

// 判断字符串是否是ascii
bool IsByteAscii(byte byAsc)
{
	return (byAsc >= 32 && byAsc <= 127) || _ismbcprint((byAsc) != 0);
// 	return (byAsc >= 32 && byAsc <= 127) 
// 		|| _ismbcprint((byAsc) != 0 
// 		|| byAsc == 0x0d 
// 		|| byAsc == 0x0a
// 		|| byAsc == 0x09);
}

/*
bool  IsByteAscii(byte byAsc)
{
	if (_ismbcprint((byAsc) != 0))
	{
		return true;
	}
	return true;
}
*/

DWORD GetUnicodeAsciiLen(const BYTE* pMem, DWORD dwBufLen)
{
    DWORD dwReturnLen = 0;

    for (dwReturnLen = 0; dwReturnLen < dwBufLen; dwReturnLen += 2)
    {
        if (pMem[dwReturnLen + 1] != 0)
        {
            break;
        }
        else
        {
            if (pMem[dwReturnLen] == 0)
            {
                break;
            }
            else if (!IsByteAscii(pMem[dwReturnLen]) && pMem[dwReturnLen] != 0x20)  // 判断pMem[i]是否是字符串或空格
            {
                break;
            }
        }
    }

    return dwReturnLen;
}

DWORD IsAscii( BYTE* pbMem,BOOL& bAscii )
{
	bAscii = false;
	WORD wChar = 0;
	DWORD dwLen = 0;

	try
	{
		do 
		{
			wChar = *(WORD*)(pbMem + dwLen);
			if (IsSimplifiedCH(wChar))
			{
				dwLen += 2;
			}
			else if (_ismbcprint((byte)wChar) !=0 ||
					(byte)wChar == 0x0d ||
					(byte)wChar == 0x0a ||
					(byte)wChar == 0x09)
			{
				dwLen++;
			}
			else
			{
				if (dwLen != 0 && (byte)wChar == 0)
				{
					dwLen++;
					bAscii = true;
				}

				break;
			}
		} while (true);
	}
	catch(...)
	{
		return 0;
	}

	return dwLen;
}

// 判断是否是字符,包括中文字符
bool IsAsciiOrUnicodeString(byte* pbMem, DWORD& dwLen, BOOL& bIsUnicode, DWORD dwMaxCount)
{
	bool bAscii = false;
	byte byChar = 0;
	WORD wChar = 0;

	dwLen = 0;
    bIsUnicode = FALSE;

    try
    {
        dwLen = GetUnicodeAsciiLen(pbMem, dwMaxCount);
    }
    catch(...)
    {
    }

    if (dwLen > 10)
    {
        bIsUnicode = TRUE;
        return true;
    }
	dwLen = 0;
	try
	{
		do 
		{
			if (dwLen >= dwMaxCount)
			{
                bAscii = true;
				break;
			}

			byChar = pbMem[dwLen];
			wChar = *(WORD*)(pbMem + dwLen);
			if (IsSimplifiedCH(wChar))
			{
				dwLen += 2;
			}
			else if (IsByteAscii(byChar))
			{
				dwLen++;
			}
			else
			{
				if (dwLen != 0 && (((byte)wChar == 0) 
					|| (byte)wChar  == 0x0d 
					|| (byte)wChar  == 0x0a
					|| (byte)wChar  == 0x09
					|| wChar == 0x0a0d))
//				if (dwLen != 0 && (((byte)wChar == 0)))
				{
					bAscii = true;
				}
				else
				{
					dwLen = 0;
				}

				break;
			}
		} while (true);
	}
	catch(...)
	{
        bAscii = false;
	}

	return bAscii;
}


// 判断是否是字符,包括中文字符
BOOL IsAsciiOrUnicodeStringEx(byte* pbMem,
                              DWORD dwMemLen, 
                              DWORD& dwLen, 
                              BOOL& bIsUnicode)
{
    BOOL bAscii = FALSE;
    byte byChar = 0;
    WORD wChar = 0;

    dwLen = 0;
    bIsUnicode = FALSE;

    try
    {
        dwLen = GetUnicodeAsciiLen(pbMem, dwMemLen);
    }
    catch(...)
    {
    }

    if (dwLen > 10)
    {
        bIsUnicode = TRUE;
        return TRUE;
    }

    //
    // 非UNICODE字符串
    //

    dwLen = 0;

    while (dwLen < dwMemLen)
    {
        if (dwLen > 4096)
        {
            break;
        }

        byChar = pbMem[dwLen];

        if (dwLen < dwMemLen - 1)
        {
            //
            // 还剩下超过2字节,需要判断是否是SimplifiedCH
            //

            wChar = *(WORD*)(pbMem + dwLen);

            if (IsSimplifiedCH(wChar))
            {
                //
                // 是SimplifiedCH,长度直接加2,进行下一轮判断
                //
                dwLen += 2;
                continue;
            }
        }
        else
        {
            //
            // 只剩下一个字节,则设置wChar为最后一个字节以便后续判断
            //

            wChar = byChar;
        }

        if (IsByteAscii(byChar))
        {
            dwLen++;
        }
        else
        {
            if (dwLen != 0 && (((byte)wChar == 0) 
                || (byte)wChar == 0x0d 
                || (byte)wChar == 0x0a
                || (byte)wChar == 0x09
                || wChar == 0x0a0d))
            {
                dwLen++;
                bAscii = TRUE;
            }
            else
            {
                dwLen = 0;
            }

            break;
        }
    }

    if (dwLen == dwMemLen)
    {
        //
        // 如果所有的byte都是ascii或SimplifiedCH,则设置结果为TRUE
        //

        bAscii = TRUE;
    }

    return bAscii;
}
