/*************************************/
/*	file: SHA1.c							*/
/*	description: SHA1 encryption algorithm	*/
/*	history: pangtaibao created at 20121206*/
/*************************************/

#include "sha1.h"
#include <string.h>
#include "stdsoap2.h"
#include "common.h"


/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits,word) \
                (((word) << (bits)) | ((word) >> (32-(bits))))

#ifdef __cplusplus
extern "C" {
#endif

//结构体初始化
int SHA1Init(SHA1Context *_pcContext)
{
	SHA1Context *pstContext = NULL;

	pstContext = _pcContext;
	
    if(NULL == pstContext)
    {
        return shaNull;
    }

    pstContext->Length_Low             = 0;
    pstContext->Length_High            = 0;
    pstContext->Message_Block_Index    = 0;

    pstContext->Intermediate_Hash[0]   = 0x67452301;
    pstContext->Intermediate_Hash[1]   = 0xEFCDAB89;
    pstContext->Intermediate_Hash[2]   = 0x98BADCFE;
    pstContext->Intermediate_Hash[3]   = 0x10325476;
    pstContext->Intermediate_Hash[4]   = 0xC3D2E1F0;

    pstContext->Computed   = 0;
    pstContext->Corrupted  = 0;

    return shaSuccess;
}

int SHA1Input(SHA1Context *_pcContext, const unsigned char *_pucMessageArray, unsigned int _iLength)
{
	SHA1Context *pstContext = NULL;
	int iLength = 0;

	pstContext 	= _pcContext;
	iLength 	= _iLength;

    if(0 == iLength || NULL == pstContext || NULL == _pucMessageArray)
    {
        return shaSuccess;
    }

	
    while(iLength-- && 0 == pstContext->Corrupted)
    {
    	pstContext->Message_Block[pstContext->Message_Block_Index++] = (*_pucMessageArray & 0xFF);

    	pstContext->Length_Low += 8;

		if (pstContext->Length_Low == 0)
	    {
	        pstContext->Length_High++;
	        if (0 == pstContext->Length_High)
	        {
	            /* Message is too long */
	            pstContext->Corrupted = 1;
	        }
	    }

	    if (pstContext->Message_Block_Index == 64)
	    {
	        SHA1ProcessMessageBlock(pstContext);
	    }

    	_pucMessageArray++;
    }

    return shaSuccess;
}

void SHA1ProcessMessageBlock(SHA1Context *_pcContext)
{
    const unsigned int K[] =    {       /* Constants defined in SHA-1   */
                            0x5A827999,
                            0x6ED9EBA1,
                            0x8F1BBCDC,
                            0xCA62C1D6
                            };
    int           	t;                 /* Loop counter                */
    unsigned int    temp;              /* Temporary word value   */
    unsigned int    W[80];             /* Word sequence             */
    unsigned int    A, B, C, D, E;     /* Word buffers                */

    /*
     *  Initialize the first 16 words in the array W
     */
    for(t = 0; t < 16; t++)
    {
        W[t] = _pcContext->Message_Block[t * 4] << 24;
        W[t] |= _pcContext->Message_Block[t * 4 + 1] << 16;
        W[t] |= _pcContext->Message_Block[t * 4 + 2] << 8;
        W[t] |= _pcContext->Message_Block[t * 4 + 3];
    }

    for(t = 16; t < 80; t++)
    {
       W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = _pcContext->Intermediate_Hash[0];
    B = _pcContext->Intermediate_Hash[1];
    C = _pcContext->Intermediate_Hash[2];
    D = _pcContext->Intermediate_Hash[3];
    E = _pcContext->Intermediate_Hash[4];

    for(t = 0; t < 20; t++)
    {
        temp =  SHA1CircularShift(5,A) +
                ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);

        B = A;
        A = temp;
    }

    for(t = 20; t < 40; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 40; t < 60; t++)
    {
        temp = SHA1CircularShift(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 60; t < 80; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    _pcContext->Intermediate_Hash[0] += A;
    _pcContext->Intermediate_Hash[1] += B;
    _pcContext->Intermediate_Hash[2] += C;
    _pcContext->Intermediate_Hash[3] += D;
    _pcContext->Intermediate_Hash[4] += E;

    _pcContext->Message_Block_Index = 0;
}

int SHA1Result(SHA1Context *_pcContext, unsigned char *_pucDigest)
{
	SHA1Context *pstContext 	= NULL;
	unsigned char *pucDigest 	= NULL;
    int i = 0;

	pstContext 	= _pcContext;
	pucDigest 	= _pucDigest;


    if (NULL == pstContext || NULL == pucDigest)
    {
        return shaNull;
    }

    if (0 == pstContext->Computed)
    {
        SHA1PadMessage(pstContext);
        for(i=0; i<64; ++i)
        {
            /* message may be sensitive, clear it out */
            pstContext->Message_Block[i] = 0;
        }
		/* and clear length */
        pstContext->Length_Low = 0;    
        pstContext->Length_High = 0;
        pstContext->Computed = 1;

    }

    for(i = 0; i < SOAP_SMD_SHA1_SIZE; ++i)
    {
        pucDigest[i] = pstContext->Intermediate_Hash[i>>2]
                            >> 8 * ( 3 - ( i & 0x03 ) );
    }

    return shaSuccess;
}

void SHA1PadMessage(SHA1Context *_pcContext)
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second
     *  block.
     */
	SHA1Context *pstContext 	= NULL;
	
	pstContext	= _pcContext;
	
    if (NULL == pstContext)
    {
        return;
    }

    if (pstContext->Message_Block_Index > 55)
    {
        pstContext->Message_Block[pstContext->Message_Block_Index++] = 0x80;
        while(pstContext->Message_Block_Index < 64)
        {
            pstContext->Message_Block[pstContext->Message_Block_Index++] = 0;
        }

        SHA1ProcessMessageBlock(pstContext);

        while(pstContext->Message_Block_Index < 56)
        {
            pstContext->Message_Block[pstContext->Message_Block_Index++] = 0;
        }
    }
    else
    {
        pstContext->Message_Block[pstContext->Message_Block_Index++] = 0x80;
        while(pstContext->Message_Block_Index < 56)
        {

            pstContext->Message_Block[pstContext->Message_Block_Index++] = 0;
        }
    }

    /*
     *  Store the message length as the last 8 octets
     */
    pstContext->Message_Block[56] = pstContext->Length_High >> 24;
    pstContext->Message_Block[57] = pstContext->Length_High >> 16;
    pstContext->Message_Block[58] = pstContext->Length_High >> 8;
    pstContext->Message_Block[59] = pstContext->Length_High;
    pstContext->Message_Block[60] = pstContext->Length_Low >> 24;
    pstContext->Message_Block[61] = pstContext->Length_Low >> 16;
    pstContext->Message_Block[62] = pstContext->Length_Low >> 8;
    pstContext->Message_Block[63] = pstContext->Length_Low;

    SHA1ProcessMessageBlock(pstContext);
}

void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE])
{
	SHA1Context context;
	
	if(NULL == created || NULL == nonce || NULL == password || NULL == hash)
	{
		return;
	}

	SHA1Init(&context);
	SHA1Input(&context, (unsigned char *)nonce, noncelen);
	SHA1Input(&context, (unsigned char *)created, strlen(created));
	SHA1Input(&context, (unsigned char *)password, strlen(password));
	SHA1Result(&context, (unsigned char *)hash);
}

#ifdef __cplusplus
}
#endif

