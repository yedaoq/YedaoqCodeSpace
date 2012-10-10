/* ____________________________________________________________________________
@ 
@ file - BaseXXCoder.h
@ 
@ auth - yedaoq@gmail.com            http://blog.yedaoq.info
@ date - 2012-1-10
@ info -
@     Binary coder : BASE64,BASE24,BASE16
/* ____________________________________________________________________________*/
#pragma once

#include <string>
typedef unsigned char byte;

class CBase64Coder
{
public:

    // convert from Base64 to ANSI
    static std::string encode(const std::string in_str);

    // convert from ANSI to Base64
    static std::string decode(const std::string in_str);

private:

    // encode table
    const static std::string _base64_encode_chars;
 
    // decode table
    const static char _base64_decode_chars[128];
};

class CBase24Coder
{
public:
	template<typename iter_t>
	int decode(byte* pBuffer, unsigned int iBufferLen, iter_t iInStr, iter_t iInStrEnd);

	template<typename iter_t>
	int decode(byte* pBuffer, unsigned int iBufferLen, iter_t iInStr);

	template<typename iter_t>
	int encode(const byte* pData, unsigned int iDataLen, iter_t iBuf, iter_t iBufEnd);

	template<typename iter_t>
	int encode(const byte* pData, unsigned int iDataLen, iter_t iBuf );

public:
	CBase24Coder(const char* charset = 0);
	CBase24Coder(const wchar_t* charset);

	const char* GetCharset() const { return EncodeCharset; } 
	bool SetCharset(int idx, const char c);

protected:
	char		EncodeCharset[24];
	signed char DecodeCharidx[256];

	void Initialize();

public:
	static const char DefaultEncodeCharset[25];
};

class CBase16Coder
{
public:
	template<typename iter_t>
	int decode(byte* pBuffer, unsigned int iBufferLen, iter_t iInStr, iter_t iInStrEnd);

	template<typename iter_t>
	int decode(byte* pBuffer, unsigned int iBufferLen, iter_t iInStr);

	template<typename iter_t>
	int encode(const byte* pData, unsigned int iDataLen, iter_t iBuf, iter_t iBufEnd);

	template<typename iter_t>
	int encode(const byte* pData, unsigned int iDataLen, iter_t iBuf );

public:
	CBase16Coder(bool bUpper = true);

protected:
	const char * EncodeCharset;

public:
	static const char			EncodeCharsetU[16];
	static const char			EncodeCharsetL[16];
	static const signed char	DecodeCharidx[256];
};

template<typename iter_t>
int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, iter_t iBuf )
{
	byte temp, baseidx = 0;

	if(!pData || iDataLen <= 0) return ERROR_INVALID_PARAMETER;

	const byte* pDataTmp = pData, *pDataEnd = pData + iDataLen;

	while(pDataTmp < pDataEnd)
	{
		temp = (*pDataTmp)>>4;
		*iBuf++ = temp + baseidx;
		baseidx = temp & 0x07;

		temp = (*pDataTmp)&0x0F;
		*iBuf++ = temp + baseidx;
		baseidx = temp & 0x07;
		++pDataTmp;
	}

	return pDataTmp - pData;
}

template<typename iter_t>
int CBase24Coder::encode( const byte* pData, unsigned int iDataLen, iter_t iBuf, iter_t iBufEnd )
{
	byte temp, baseidx = 0;

	if(!pData || iDataLen <= 0) return ERROR_INVALID_PARAMETER;

	const byte* pDataTmp = pData, *pDataEnd = pData + iDataLen;

	while(pDataTmp < pDataEnd)
	{
		if(iBuf == iBufEnd) break;

		temp = (*pDataTmp)>>4;
		*iBuf++ = temp + baseidx;
		baseidx = temp & 0x07;

		if(iBuf == iBufEnd) break;
		temp = (*pDataTmp)&0x0F;
		*iBuf++ = temp + baseidx;
		baseidx = temp & 0x07;
		++pDataTmp;
	}

	return pDataTmp - pData;
}

template<typename iter_t>
int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, iter_t iInStr )
{
	signed char temp;
	byte baseidx = 0;

	if(!pBuffer || iBufferLen <= 0) return ERROR_INVALID_PARAMETER;

	byte *pBufTmp = pBuffer, *pBufEnd = pBuffer + iBufferLen;

	while (pBufTmp != pBufEnd)
	{
		// 高4位
		temp = DecodeCharidx[(byte)*iInStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp = temp << 4;

		// 低4位
		temp = DecodeCharidx[(byte)*iInStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp++ |= temp;
	}

	return pBufTmp - pBuffer;
}

template<typename iter_t>
int CBase24Coder::decode( byte* pBuffer, unsigned int iBufferLen, iter_t iInStr, iter_t iInStrEnd )
{
	signed char temp;
	byte baseidx = 0;

	if(!pBuffer || iBufferLen <= 0) return ERROR_INVALID_PARAMETER;

	byte *pBufTmp = pBuffer, *pBufEnd = pBuffer + iBufferLen;

	while (iInStr != iInStrEnd && pBufTmp != pBufEnd)
	{
		// 高4位
		temp = DecodeCharidx[(byte)*iInStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp = temp << 4;

		if(iInStr == iInStrEnd) break;

		// 低4位
		temp = DecodeCharidx[(byte)*iInStr++] - baseidx;
		baseidx = temp & 0x07;

		if(temp >= 16 || temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp++ |= temp;
	}

	return pBufTmp - pBuffer;
}

template<typename iter_t>
int CBase16Coder::encode( const byte* pData, unsigned int iDataLen, iter_t iBuf )
{
	if(!pData || iDataLen <= 0) return MYERROR_INVALID_PARA;

	const byte* pDataTmp = pData, *pDataEnd = pData + iDataLen;

	while (pDataTmp < pDataEnd)
	{
		*iBuf++ = EncodeCharset[pDataTmp >> 4];
		*iBuf++ = EncodeCharset[pDataTmp & 0x0F];
		++pDataTmp;
	}

	return pDataTmp - pData;
}

template<typename iter_t>
int CBase16Coder::encode( const byte* pData, unsigned int iDataLen, iter_t iBuf, iter_t iBufEnd )
{
	if(!pData || iDataLen <= 0) return MYERROR_INVALID_PARA;

	const byte* pDataTmp = pData, *pDataEnd = pData + iDataLen;

	while (pDataTmp < pDataEnd && iBuf != iBufEnd)
	{
		*iBuf++ = EncodeCharset[pDataTmp >> 4];
		if(iBuf == iBufEnd) break;
		*iBuf++ = EncodeCharset[pDataTmp & 0x0F];
		++pDataTmp;
	}

	return pDataTmp - pData;
}

template<typename iter_t>
int CBase16Coder::decode( byte* pBuffer, unsigned int iBufferLen, iter_t iInStr )
{
	if(!pBuffer || iBufferLen <= 0) return ERROR_INVALID_PARAMETER;

	signed char temp;
	byte *pBufTmp = pBuffer, *pBufEnd = pBuffer + iBufferLen;

	while(pBufTmp < pBufEnd)
	{
		temp = DecodeCharidx[*iInStr++];
		if(temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp = temp << 4;

		temp = DecodeCharidx[*iInStr++];
		if(temp < 0) return ERROR_INVALID_DATA;
		*pBufTmp++ |= temp;
	}

	return pBufTmp - pBuffer;
}

template<typename iter_t>
int CBase16Coder::decode( byte* pBuffer, unsigned int iBufferLen, iter_t iInStr, iter_t iInStrEnd )
{
	if(!pBuffer || iBufferLen <= 0) return MYERROR_INVALID_PARA;

	signed char temp;
	byte *pBufTmp = pBuffer, *pBufEnd = pBuffer + iBufferLen;

	while(pBufTmp < pBufEnd && iInStr != iInStrEnd)
	{
		temp = DecodeCharidx[*iInStr++];
		if(temp < 0) return MYERROR_INVALID_DATA;
		*pBufTmp = temp << 4;

		if(iInStr == iInStrEnd) break;
		temp = DecodeCharidx[*iInStr++];
		if(temp < 0) return MYERROR_INVALID_DATA;
		*pBufTmp++ |= temp;
	}

	return pBufTmp - pBuffer;
}


//class CBinaryCoder
//{
//public:
//	int decode(byte* pBuffer, unsigned int iBufferLen, const char* str);
//	int	decode(byte* pBuffer, unsigned int iBufferLen, const wchar_t* str);
//	int decode(byte* pBuffer, unsigned int iBufferLen, const std::string& str);
//	int	decode(byte* pBuffer, unsigned int iBufferLen, const std::wstring& str);
//
//	int	encode(const byte* pData, unsigned int iDataLen, char* buf);
//	int	encode(const byte* pData, unsigned int iDataLen, wchar_t* buf);
//	int	encode(const byte* pData, unsigned int iDataLen, std::string& buf);
//	int	encode(const byte* pData, unsigned int iDataLen, std::wstring& buf);
//};

//enum EnumBinaryCoderAlgo
//{
//	BASE24,
//	BASE64,
//	BASE16,
//};
//
//struct BinaryCoderContext
//{
//	BinaryCoderContext(EnumBinaryCoderAlgo algo)
//};
