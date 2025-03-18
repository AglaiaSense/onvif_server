/*
 *	file: Wsseapi.h
 *	description: Wsseapi.h
 *	history: pangtaibao created at 20120405
 */

#ifndef WSSEAPI_H
#define WSSEAPI_H

#include "onvifH.h"	/* replace with soapcpp2-generated *H.h file */
#include "sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

int soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce);
void soap_wsse_session_cleanup(struct soap *soap);
//void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE]);
int soap_wsse_verify_Password(struct soap *soap, const char *password);
void soap_wsse_delete_Security(struct soap *soap);
int soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail);
int soap_wsse_fault(struct soap *soap, enum wsse__FaultcodeEnum fault, const char *detail);

struct _wsse__Security* soap_wsse_Security(struct soap *soap);
struct _wsse__UsernameToken *soap_wsse_UsernameToken(struct soap *soap, const char *id);

#ifdef __cplusplus
}
#endif

#endif

