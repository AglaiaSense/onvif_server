/*************************************/
/*	file: SHA1.H							 */
/*	description: SHA1 encryption algorithm   */
/*	history: pangtaibao created at 20121206 */
/*************************************/

#ifndef SHA1_H
#define SHA1_H

#include "stdsoap2.h"

/** SHA1 digest size in octets */
#define SOAP_SMD_SHA1_SIZE	(20)

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError       /* called Input after Result */
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SHA1Context
{
    unsigned int Intermediate_Hash[SOAP_SMD_SHA1_SIZE/4]; 
    unsigned int Length_Low;            
    unsigned int Length_High;          
    unsigned int Message_Block_Index;
    unsigned char Message_Block[64];      
    int Computed;               
    int Corrupted;            
} SHA1Context;

int SHA1Init(SHA1Context *_pcContext);

int SHA1Input(SHA1Context *_pcContext, const unsigned char *_pucMessageArray, unsigned int _iLength);

int SHA1Result(SHA1Context *_pcContext, unsigned char *_pucDigest);

void SHA1ProcessMessageBlock(SHA1Context *_pcContext);

void SHA1PadMessage(SHA1Context *_pcContext);

void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE]);

#ifdef __cplusplus
}
#endif

#endif

