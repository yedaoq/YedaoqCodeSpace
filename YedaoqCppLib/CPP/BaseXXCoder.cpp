#include "BaseXXCoder.h"
#include "ErrorCode.h"

using namespace std;

const std::string CBase64Coder::_base64_encode_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const char CBase64Coder::_base64_decode_chars[] = 
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

std::string CBase64Coder::encode(const std::string in_str)
{
    std::string out_str;
    unsigned char c1, c2, c3;
    int i = 0;
    int len = in_str.length();

    while ( i<len )
    {
        // read the first byte
        c1 = in_str[i++];
        if ( i==len )       // pad with "="
        {
            out_str += _base64_encode_chars[ c1>>2 ];
            out_str += _base64_encode_chars[ (c1&0x3)<<4 ];
            out_str += "==";
            break;
        }

        // read the second byte
        c2 = in_str[i++];
        if ( i==len )       // pad with "="
        {
            out_str += _base64_encode_chars[ c1>>2 ];
            out_str += _base64_encode_chars[ ((c1&0x3)<<4) | ((c2&0xF0)>>4) ];
            out_str += _base64_encode_chars[ (c2&0xF)<<2 ];
            out_str += "=";
            break;
        }

        // read the third byte
        c3 = in_str[i++];
        // convert into four bytes string
        out_str += _base64_encode_chars[ c1>>2 ];
        out_str += _base64_encode_chars[ ((c1&0x3)<<4) | ((c2&0xF0)>>4) ];
        out_str += _base64_encode_chars[ ((c2&0xF)<<2) | ((c3&0xC0)>>6) ];
        out_str += _base64_encode_chars[ c3&0x3F ];
    }

    return out_str;
}

std::string CBase64Coder::decode(const std::string in_str)
{
    std::string out_str;
    char c1, c2, c3, c4;
    int i = 0;
    int len = in_str.length();

    while ( i<len)
    {
        // read the first byte
        do {
            c1 = _base64_decode_chars[ in_str[i++] ];
        } while ( i<len && c1==-1);

        if ( c1==-1)
            break;

        // read the second byte
        do {
            c2 = _base64_decode_chars[ in_str[i++] ];
        } while ( i<len && c2==-1);

        if ( c2==-1 )
            break;

        // assamble the first byte
        out_str += char( (c1<<2) | ((c2&0x30)>>4) );

        // read the third byte
        do {
            c3 = in_str[i++];
            if ( c3==61 )       // meet with "=", break
                return out_str;
            c3 = _base64_decode_chars[ c3 ];
        } while ( i<len && c3==-1);

        if ( c3==-1 )
            break;

        // assamble the second byte
        out_str += char( ((c2&0XF)<<4) | ((c3&0x3C)>>2) );

        // read the fourth byte
        do {
            c4 = in_str[i++];
            if ( c4==61 )       // meet with "=", break
                return out_str;
            c4 = _base64_decode_chars[ c4 ];
        } while ( i<len && c4==-1 );

        if ( c4==-1 )
            break;

        // assamble the third byte
        out_str += char( ((c3&0x03)<<6) | c4 );
    }

    return out_str;
}


//////////////////////////////////////////////////////////////////////////
// BASE24
//////////////////////////////////////////////////////////////////////////

const char CBase24Coder::DefaultEncodeCharset[] = {"2346789ABCEFGHJKMPQRTWXY"};

struct IOutputStringBufProxy
{
	virtual void Append(short ch);
};

struct COutputCstrBufProxy : public IOutputStringBufProxy
{
	COutputCstrBufProxy(char* buf) : Buf(buf) {}

	virtual void Append(short ch)
	{
		*Buf++ = ch;
	}

protected:
	char* Buf;
};

struct COutputWcstrBufProxy : public IOutputStringBufProxy
{
	COutputWcstrBufProxy(wchar_t* buf) : Buf(buf) {}

	virtual void Append(short ch)
	{
		*Buf++ = ch;
	}

protected:
	wchar_t* Buf;
};

struct COutputStrBufProxy : public IOutputStringBufProxy
{
	COutputStrBufProxy(string* pbuf) : Buf(pbuf) {}

	virtual void Append(short ch)
	{
		Buf->push_back(ch);
	}

protected:
	string* Buf;
};

struct COutputWstrBufProxy : public IOutputStringBufProxy
{
	COutputWstrBufProxy(wstring* pbuf) : Buf(pbuf) {}

	virtual void Append(short ch)
	{
		Buf->push_back(ch);
	}

protected:
	wstring* Buf;
};

template<typename iter_t>
inline int decode_base24( byte* pBuffer, unsigned int iBufferLen, iter_t begin, iter_t end, const signed char* decodecharidx )
{
	signed char temp;
	byte baseidx = 0;

	if(!pBuffer || iBufferLen <= 0) return MYERROR_INVALID_PARA;

	iter_t iterStr = begin;
	byte *pBufTmp = pBuffer, *pBufEnd = pBuffer + iBufferLen;

	while (iterStr != end && pBufTmp != pBufEnd)
	{
		// ¸ß4Î»
		temp = decodecharidx[(byte)*iterStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return MYERROR_INVALID_DATA;
		*pBufTmp = temp << 4;

		if(iterStr == end) return MYERROR_INVALID_DATA;

		// µÍ4Î»
		temp = decodecharidx[(byte)*iterStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return MYERROR_INVALID_DATA;
		*pBufTmp++ |= temp;
	}

	return MYERROR_SUCCESS;
}

inline int encode_base24(const byte* pData, unsigned int iDataLen, IOutputStringBufProxy& buf, const char* encodeentry )
{
	byte temp, baseidx = 0;

	if(!pData || iDataLen <= 0 || !encodeentry) return MYERROR_INVALID_PARA;

	const byte* pDataTmp = pData, *pDataEnd = pData + iDataLen;

	while(pDataTmp < pDataEnd)
	{
		temp = (*pDataTmp)>>4;
		buf.Append(encodeentry[temp + baseidx]);
		baseidx = temp & 0x07;

		temp = (*pDataTmp)&0x0F;
		buf.Append(encodeentry[temp + baseidx]);
		baseidx = temp & 0x07;
		++pDataTmp;
	}

	return MYERROR_SUCCESS;
}

//int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, const char* str )
//{
//	if(0 == str) return MYERROR_INVALID_PARA;
//	int len = strnlen(str, iBufferLen * 2);
//	return decode_base24(pBuffer, iBufferLen, str, str + len, this->DecodeCharidx);
//}
//
//int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, const wchar_t* str )
//{
//	if(0 == str) return MYERROR_INVALID_PARA;
//	int len = wcsnlen(str, iBufferLen * 2);
//	return decode_base24(pBuffer, iBufferLen, str, str + len, this->DecodeCharidx);
//}
//
//int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, const string& str )
//{
//	return decode_base24(pBuffer, iBufferLen, str.begin(), str.end(), this->DecodeCharidx);
//}
//
//int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, const wstring& str )
//{
//	return decode_base24(pBuffer, iBufferLen, str.begin(), str.end(), this->DecodeCharidx);
//}
//
//int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, string& buf )
//{
//	return encode_base24(pData, iDataLen, COutputStrBufProxy(&buf), this->EncodeCharset);
//}
//
//int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, wstring& buf )
//{
//	return encode_base24(pData, iDataLen, COutputWstrBufProxy(&buf), this->EncodeCharset);
//}
//
//int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, char* buf )
//{
//	return encode_base24(pData, iDataLen, COutputCstrBufProxy(buf), this->EncodeCharset);
//}
//
//int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, wchar_t* buf )
//{
//	return encode_base24(pData, iDataLen, COutputWcstrBufProxy(buf), this->EncodeCharset);
//}

CBase24Coder::CBase24Coder( const char* charset )
{
	if(charset)
	{
		if(strlen(charset) < 24)
		{
			throw MYERROR_INVALID_PARA;
		}
	}
	else
	{
		charset = DefaultEncodeCharset;
	}

	memcpy(EncodeCharset, charset, sizeof(EncodeCharset));
	Initialize();
}

CBase24Coder::CBase24Coder( const wchar_t* charset )
{
	if(charset)
	{
		if(wcslen(charset) < 24)
		{
			throw MYERROR_INVALID_PARA;
		}
		else
		{
			for(int i = 0; i < 24; ++i) EncodeCharset[i] = *charset++;
		}
	}
	else
	{
		memcpy(EncodeCharset, charset, sizeof(EncodeCharset));
	}
	
	Initialize();
}

void CBase24Coder::Initialize()
{
	memset(DecodeCharidx, -1, sizeof(DecodeCharidx));
	for(int i = 0; i < 24; ++i)
	{
		if(DecodeCharidx[EncodeCharset[i]] != -1)
		{
			throw MYERROR_INVALID_PARA;
		}
		DecodeCharidx[EncodeCharset[i]] = i;
	}
}

bool CBase24Coder::SetCharset( int idx, const char c )
{
	if(c == EncodeCharset[idx]) return true;
	if(DecodeCharidx[c] != -1) return false;
	DecodeCharidx[EncodeCharset[idx]] = -1;
	DecodeCharidx[c] = idx;
	EncodeCharset[idx] = c;
	return true;
}


//////////////////////////////////////////////////////////////////////////
// BASE16
//////////////////////////////////////////////////////////////////////////
const char CBase16Coder::EncodeCharsetU[] = 
{
	'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'	
};

const char CBase16Coder::EncodeCharsetL[] = 
{
	'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'	
};

const signed char CBase16Coder::DecodeCharidx[] = 
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};



CBase16Coder::CBase16Coder( bool bUpper /*= true*/ )
{
	EncodeCharset = bUpper ? EncodeCharsetU : EncodeCharsetL;
}

//CBase16Coder::DecodeCharIdxCollection::DecodeCharIdxCollection()
//{
//	memset(Idxs, -1, sizeof(Idxs));
//	signed char* tmp = Idxs + '0' - 1;
//	for (int i = 0; i < 10; ++i) *(++tmp) = i;
//	tmp = Idxs + 'A' - 1;
//	for (int i = 10; i < 16; ++i) *(++tmp) = i;
//	tmp = Idxs + 'a' - 1;
//	for (int i = 10; i < 16; ++i) *(++tmp) = i;
//}

