/*
 *	file: wsaapi.c
 *	description: wsaapi functions
 *	history: pangtaibao created at 20120405
 */
#include "wsaapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Plugin identification for plugin registry */
const char soap_wsa_id[] = SOAP_WSA_ID;

#if defined(SOAP_WSA_2003)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2003/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2003/03/addressing/fault";
#elif defined(SOAP_WSA_2004)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2004/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2004/03/addressing/fault";
#elif defined(SOAP_WSA_2005)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://www.w3.org/2005/08/addressing/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "http://www.w3.org/2005/08/addressing/none";
const char *soap_wsa_faultAction = "http://www.w3.org/2005/08/addressing/soap/fault";
#else
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2004/08/addressing/fault";
#endif

/**
@fn const char *soap_wsa_rand_uuid(struct soap *soap)
@brief Generates a random UUID (UUID algorithm version 4). Compile all source
codes with -DWITH_OPENSSL for better randomness results.
@param soap context
@return UUID "urn:uuid:xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"
*/
const char*
soap_wsa_rand_uuid(struct soap *soap)
{
	char *uuid = (char*)soap_malloc(soap, 48);
	int r1, r2, r3, r4;
	int *pbuf = NULL;
	int iTemp = 0;
#ifdef WITH_OPENSSL
	r1 = soap_random;
	r2 = soap_random;
#else
	static int k = 0xFACEB00B;
	int lo = k % 127773;
	int hi = k / 127773;
#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	gettimeofday(&tv, NULL);
	r1 = 10000000 * tv.tv_sec + tv.tv_usec;
#else
	r1 = (int)time(NULL);
#endif
	k = 16807 * lo - 2836 * hi;
	if(k <= 0)
		k += 0x7FFFFFFF;
	r2 = k;
	k &= 0x8FFFFFFF;
	pbuf = (int*)soap->buf;
	iTemp = pbuf[0];
	r2 += iTemp;
#endif
	r3 = soap_random;
	r4 = soap_random;
	//sprintf(uuid, "urn:uuid:%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x", r1, (short)(r2 >> 16), (short)r2 >> 4, ((short)(r3 >> 16) & 0x3FFF) | 0x8000, (short)r3, r4);
	sprintf(uuid, "urn:uuid:%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x", r1, (short)(r2 >> 16), ((short)r2 >> 4 & 0xFFF), ((short)(r3 >> 16) & 0x3FFF) | 0x8000, (short)r3, r4);	//dlq modified for sony search at 20130719
	DBGFUN1("soap_wsa_rand_uuid", "%s", uuid);
	return uuid;
}

/******************************************************************************\
 *
 *	Client-side Request
 *
\******************************************************************************/

/**
@fn int soap_wsa_request(struct soap *soap, const char *id, const char *to, const char *action)
@brief Sets the WS-Addressing information header for the next request message
with MessageID (optional), To (optional), and Action (required).
@param soap context
@param[in] faultsubcode sub code string
@param[in] faultstring fault string
@param[in] faultdetail detail string
@return SOAP_FAULT
*/
int
soap_wsa_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail)
{ 
	return soap_wsa_fault_subcode(soap, 1, faultsubcode, faultstring, faultdetail);
}

/******************************************************************************\
 *
 *	Server-side SOAP Fault
 *
\******************************************************************************/

/**
@fn int soap_wsa_fault_subcode(struct soap *soap, int flag, const char *faultsubcode, const char *faultstring, const char *faultdetail)
@brief Sets sender/receiver SOAP Fault (sub)code for server fault response.
@param soap context
@param[in] flag 0=receiver, 1=sender
@param[in] faultsubcode sub code string
@param[in] faultstring fault string
@param[in] faultdetail detail string
@return SOAP_FAULT
*/
int
soap_wsa_fault_subcode(struct soap *soap, int flag, const char *faultsubcode, const char *faultstring, const char *faultdetail)
{ 
	return soap_wsa_fault_subcode_action(soap, flag, faultsubcode, faultstring, faultdetail, NULL); 
}
/**
@fn int soap_wsa_response(struct soap *soap, int status, size_t count)
@brief Overrides the HTTP response operations to send an HTTP POST
@param soap context
@param status code
@param count message length (if non-chunked)
*/
static int
soap_wsa_response(struct soap *soap, int status, size_t count)
{ struct soap_wsa_data *data = (struct soap_wsa_data*)soap_lookup_plugin(soap, soap_wsa_id);
  DBGFUN2("soap_wsa_response", "status=%d", status, "count=%lu", (unsigned long)count);
  if (!data)
    return SOAP_PLUGIN_ERROR;
  soap->fresponse = data->fresponse;	/* reset (HTTP response) */
  return soap->fpost(soap, soap_strdup(soap, soap->endpoint), soap->host, soap->port, soap->path, soap->action, count);
}

/******************************************************************************\
 *
 *	Misc.
 *
\******************************************************************************/

/**
@fn int soap_wsa_alloc_header(struct soap *soap)
@brief Adds SOAP Header if not present.
@param soap context
@return SOAP_OK
*/
static int
soap_wsa_alloc_header(struct soap *soap)
{ soap_header(soap);
  if (soap->header)
    return SOAP_OK;
  return soap->error = SOAP_EOM;
}

/**
@fn int soap_wsa_fault_subcode_action(struct soap *soap, int flag, const char *faultsubcode, const char *faultstring, const char *faultdetail, const char *action)
@brief Sets sender/receiver SOAP Fault (sub)code and action for server fault response.
@param soap context
@param[in] flag 0=receiver, 1=sender
@param[in] faultsubcode sub code string
@param[in] faultstring fault string
@param[in] faultdetail detail string
@param[in] action WS-Addressing action string
@return SOAP_FAULT
*/
int
soap_wsa_fault_subcode_action(struct soap *soap, int flag, const char *faultsubcode, const char *faultstring, const char *faultdetail, const char *action)
{ struct soap_wsa_data *data = (struct soap_wsa_data*)soap_lookup_plugin(soap, soap_wsa_id);
  struct SOAP_ENV__Header *oldheader, *newheader;
  DBGFUN2("soap_wsa_fault_subcode", "faultsubcode=%s", faultsubcode?faultsubcode:"(null)", "faultstring=%s", faultstring?faultstring:"(null)");
  if (!data)
    return soap->error = SOAP_PLUGIN_ERROR;
  oldheader = soap->header;
  if (oldheader && oldheader->SOAP_WSA(FaultTo) && !strcmp(oldheader->SOAP_WSA(FaultTo)->Address, soap_wsa_noneURI))
    return soap_send_empty_response(soap, SOAP_OK);	/* HTTP ACCEPTED */
  soap->header = NULL;
  /* allocate a new header */
  if (soap_wsa_alloc_header(soap))
    return soap->error;
  newheader = soap->header;
  soap_default_SOAP_ENV__Header(soap, newheader); /* remove/clear SOAP Header */
  /* check header */
  if (oldheader && oldheader->SOAP_WSA(MessageID))
  { newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*)soap_malloc(soap, sizeof(SOAP_WSA_(,RelatesTo)));
    SOAP_WSA_(soap_default_,RelatesTo)(soap, newheader->SOAP_WSA(RelatesTo));
    newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
  }
  /* header->wsa__MessageID = "..."; */
  newheader->SOAP_WSA(Action) = (char*)soap_wsa_faultAction;
  if (oldheader && oldheader->SOAP_WSA(FaultTo) && oldheader->SOAP_WSA(FaultTo)->Address && strcmp(oldheader->SOAP_WSA(FaultTo)->Address, soap_wsa_anonymousURI))
  { newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(FaultTo)->Address;
    /* (re)connect to FaultTo endpoint if From != FaultTo */
    if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address || strcmp(oldheader->SOAP_WSA(From)->Address, oldheader->SOAP_WSA(FaultTo)->Address))
    { soap->keep_alive = 0;
      soap_send_empty_response(soap, SOAP_OK);	/* HTTP ACCEPTED */
      if (soap_connect(soap, newheader->SOAP_WSA(To), newheader->SOAP_WSA(Action)))
        return soap->error = SOAP_STOP;	/* nowhere to go */
      soap_set_endpoint(soap, newheader->SOAP_WSA(To));
      if (action)
        soap->action = (char*)action;
      else
        soap->action = newheader->SOAP_WSA(Action);
      data->fresponse = soap->fresponse;
      soap->fresponse = soap_wsa_response;	/* response will be a POST */
    }
  }
  else if (oldheader && oldheader->SOAP_WSA(From))
    newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
  else
    newheader->SOAP_WSA(To) = (char*)soap_wsa_anonymousURI;
  soap->header = newheader;
  if (flag)
    return soap_sender_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
  return soap_receiver_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
}

#ifdef __cplusplus
}
#endif
