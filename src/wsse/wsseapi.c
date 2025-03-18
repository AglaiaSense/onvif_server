/*
 *	file: Wsseapi.c
 *	description: Wsseapi functions
 *	history: pangtaibao created at 20120405
 */

#include "wsseapi.h"
#include "sha1.h"
#include "threads.h"	/* only need threads to enable mutex for MT */

//pangtaibao add 20120217
#include "stdsoap2.h"
//end

#if defined(SOAP_WSA_2003) || defined(SOAP_WSA_2004) || defined(SOAP_WSA_200408) || defined(SOAP_WSA_2005)
#include "wsaapi.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Clock skew between machines (in sec) to fit message expiration in window */
#define SOAP_WSSE_CLKSKEW	(300)

/** Digest authentication accepts messages that are not older than creation time + SOAP_WSSE_NONCETIME */
#define SOAP_WSSE_NONCETIME	(SOAP_WSSE_CLKSKEW + 240)

/******************************************************************************\
 *
 * Digest authentication session
 *
\******************************************************************************/

/**
@struct soap_wsse_session
@brief Digest authentication session data
*/
struct soap_wsse_session
{ struct soap_wsse_session *next;	/**< Next session in list */
  time_t expired;			/**< Session expiration */
  char hash[SOAP_SMD_SHA1_SIZE];	/**< SHA1 digest */
  char nonce[1]; /**< Nonce string flows into region below this struct */
};


/** The digest authentication session database */
static struct soap_wsse_session *soap_wsse_session = NULL;

/** Lock for digest authentication session database exclusive access */
static MUTEX_TYPE soap_wsse_session_lock = MUTEX_INITIALIZER;

/******************************************************************************\
 *
 * Common URIs
 *
\******************************************************************************/
const char *wsse_PasswordDigestURI = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";

/**
@fn int soap_wsse_verify_Password(struct soap *soap, const char *password)
@brief Verifies the supplied password or sets wsse:FailedAuthentication fault.
@param soap context
@param[in] password string to verify against
@return SOAP_OK (authorized) or SOAP_FAULT with wsse:FailedAuthentication fault

The verification supports both clear-text password verification and digest
password authentication. For digest authentication a history mechanism with a
digest authentication session database ensures protection against replay
attacks.
Note: this release supports the use of at most one UsernameToken in the header.
*/
int
soap_wsse_verify_Password(struct soap *soap, const char *password)
{ 
	_wsse__UsernameToken *token = soap_wsse_UsernameToken(soap, NULL);
	DBGFUN("soap_wsse_verify_Password");

	/* if we have a UsernameToken with a Password, check it */
	if (token && token->Password)
	{
		/* password digest or text? */
		if (token->Password->Type 
		&& !strcmp(token->Password->Type, wsse_PasswordDigestURI))
		{
			/* check password digest: compute SHA1(created, nonce, password) */
			if (token->Nonce
			&& token->wsu__Created
			&& strlen(token->Password->__item) == 28)	/* digest pw len = 28 */
			{
				char HA1[SOAP_SMD_SHA1_SIZE], HA2[SOAP_SMD_SHA1_SIZE];

				/* The specs are not clear: compute digest over binary nonce or base64 nonce? The formet appears to be the case: */
				int noncelen;
				const char *nonce = soap_base642s(soap, token->Nonce, NULL, 0, &noncelen);

				/* compute HA1 = SHA1(created, nonce, password) */
				calc_digest(soap, token->wsu__Created, nonce, noncelen, password, HA1);
				/*
				calc_digest(soap, token->wsu__Created, token->Nonce, strlen(token->Nonce), password, HA1);
				*/
				/* get HA2 = supplied digest from base64 Password */
				soap_base642s(soap, token->Password->__item, HA2, SOAP_SMD_SHA1_SIZE, NULL);

				/* compare HA1 to HA2 */
				if (!memcmp(HA1, HA2, SOAP_SMD_SHA1_SIZE))
				{
					/* authorize if HA1 and HA2 identical and not replay attack */
					//if (!soap_wsse_session_verify(soap, HA1, token->wsu__Created, token->Nonce))
					return SOAP_OK;
					//return soap->error; 
				}
			}
		}
		else
		{
			/* check password text */
			if (!strcmp(token->Password->__item, password))
				return SOAP_OK;
		}
	}

	return soap_wsse_fault(soap, wsse__FailedAuthentication, NULL);
}

/**
@fn _wsse__UsernameToken* soap_wsse_UsernameToken(struct soap *soap, const char *id)
@brief Returns UsernameToken element if present.
@param soap context
@param[in] id string of UsernameToken or NULL
@return _wsse__UsernameToken object or NULL

Note: this release supports the use of at most one UsernameToken in the header.
*/
struct _wsse__UsernameToken*
soap_wsse_UsernameToken(struct soap *soap, const char *id)
{ _wsse__Security *security = soap_wsse_Security(soap);
  if (security
   && security->UsernameToken
   && (!id || (security->UsernameToken->wsu__Id
            && !strcmp(security->UsernameToken->wsu__Id, id))))
    return security->UsernameToken;
  return NULL;
}

/**
@fn _wsse__Security* soap_wsse_Security(struct soap *soap)
@brief Returns Security header element if present.
@param soap context
@return _wsse__Security object or NULL
*/
struct _wsse__Security*
soap_wsse_Security(struct soap *soap)
{ if (soap->header)
    return soap->header->wsse__Security;
  return NULL;
}


/******************************************************************************\
 *
 * Calculate SHA1(created, nonce, password) digest
 *
\******************************************************************************/

/**
@fn static void calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE])
@brief Calculates digest value SHA1(created, nonce, password)
@param soap context
@param[in] created string (XSD dateTime format)
@param[in] nonce value
@param[in] noncelen length of nonce value
@param[in] password string
@param[out] hash SHA1 digest
*/
#if 0
void
calc_digest(struct soap *soap, const char *created, const char *nonce, int noncelen, const char *password, char hash[SOAP_SMD_SHA1_SIZE])
{ struct soap_smd_data context;
  /* use smdevp engine */
  soap_smd_init(soap, &context, SOAP_SMD_DGST_SHA1, NULL, 0);
  soap_smd_update(soap, &context, nonce, noncelen);
  soap_smd_update(soap, &context, created, strlen(created));
  soap_smd_update(soap, &context, password, strlen(password));
  soap_smd_final(soap, &context, hash, NULL);
}
#endif
/******************************************************************************\
 *
 * Digest authentication session management
 *
\******************************************************************************/

/**
@fn static int soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce)
@brief Verifies and updates the digest, nonce, and creation time against the digest authentication session database to prevent replay attacks.
@param soap context
@param[in] hash binary digest value of PasswordDigest
@param[in] created string
@param[in] nonce string (base64)
@return SOAP_OK or SOAP_FAULT
*/
int
soap_wsse_session_verify(struct soap *soap, const char hash[SOAP_SMD_SHA1_SIZE], const char *created, const char *nonce)
{ struct soap_wsse_session *session;
  time_t expired, now = time(NULL);
  DBGFUN("soap_wsse_session_verify");
  soap_s2dateTime(soap, created, &expired);
  /* creation time in the future? */
  if (expired > now + SOAP_WSSE_CLKSKEW)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, "Authorization request in future");
  expired += SOAP_WSSE_NONCETIME;
  /* expired? */
  if (expired <= now)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, "Authentication expired");
  /* purge expired messages, but don't do this all the time to improve efficiency */
  if (now % 10 == 0)
    soap_wsse_session_cleanup(soap);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Verifying session nonce=%s\n", nonce));
  /* enter mutex to check and update session */
  MUTEX_LOCK(soap_wsse_session_lock);
  for (session = soap_wsse_session; session; session = session->next)
  { if (!memcmp(session->hash, hash, SOAP_SMD_SHA1_SIZE) && !strcmp(session->nonce, nonce))
      break;
  }
  /* if not found, allocate new session data */
  if (!session)
  { session = (struct soap_wsse_session*)malloc(sizeof(struct soap_wsse_session) + strlen(nonce));
    if (session)
    { session->next = soap_wsse_session;
      session->expired = expired;
      memcpy(session->hash, hash, SOAP_SMD_SHA1_SIZE);
      strcpy(session->nonce, nonce);
      soap_wsse_session = session;
    }
    session = NULL;
  }
  /* exit mutex */
  MUTEX_UNLOCK(soap_wsse_session_lock);
  /* if replay attack, return non-descript failure */
  if (session)
    return soap_wsse_fault(soap, wsse__FailedAuthentication, NULL);
  return SOAP_OK;
}

/**
@fn static void soap_wsse_session_cleanup(struct soap *soap)
@brief Removes expired authentication data from the digest authentication session database.
@param soap context
*/
void
soap_wsse_session_cleanup(struct soap *soap)
{ struct soap_wsse_session **session;
  time_t now = time(NULL);
  DBGFUN("soap_wsse_session_cleanup");
  /* enter mutex to purge expired session data */
  MUTEX_LOCK(soap_wsse_session_lock);
  session = &soap_wsse_session;
  while (*session)
  { if ((*session)->expired < now)
    { struct soap_wsse_session *p = *session;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Deleting session nonce=%s\n", p->nonce));
      *session = p->next;
      free(p);
    }
    else
      session = &(*session)->next;
  }
  /* exit mutex */
  MUTEX_UNLOCK(soap_wsse_session_lock);
}

/**
@fn const char* soap_wsse_get_Username(struct soap *soap)
@brief Returns UsernameToken/username string or wsse:FailedAuthentication fault.
@param soap context
@return UsernameToken/username string or NULL with wsse:FailedAuthentication fault error set
@see soap_wsse_verify_Password

The returned username should be used to lookup the user's password in a
dictionary or database for server-side authentication with
soap_wsse_verify_Password.
*/
const char*
soap_wsse_get_Username(struct soap *soap)
{ _wsse__UsernameToken *token = soap_wsse_UsernameToken(soap, NULL);
  DBGFUN("soap_wsse_get_Username");
  if (token)
    return token->Username;
  soap_wsse_fault(soap, wsse__FailedAuthentication, "Username authentication required");
  return NULL;
}

/**
@fn int soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail)
@brief Sets SOAP Fault (sub)code for server response.
@param soap context
@param[in] fault is one of wsse:FaultcodeEnum
@param[in] detail string with optional text message
@return SOAP_FAULT
*/
int
soap_wsse_fault(struct soap *soap, wsse__FaultcodeEnum fault, const char *detail)
{ const char *code = soap_wsse__FaultcodeEnum2s(soap, fault);
  DBGFUN2("soap_wsse_fault", "fault=%s", code?code:"", "detail=%s", detail?detail:"");
  /* remove incorrect or incomplete Security header */
  //soap_wsse_delete_Security(soap);//ptb added at 20131008 for Admin Admin
  /* populate the SOAP Fault as per WS-Security spec */
  /* detail = NULL; */ /* uncomment when detail text not recommended */
  /* use WSA to populate the SOAP Header when WSA is used */
  switch (fault)
  { case wsse__UnsupportedSecurityToken:
      return soap_wsse_sender_fault_subcode(soap, code, "An unsupported token was provided", detail);
    case wsse__UnsupportedAlgorithm:
      return soap_wsse_sender_fault_subcode(soap, code, "An unsupported signature or encryption algorithm was used", detail);
    case wsse__InvalidSecurity:
      return soap_wsse_sender_fault_subcode(soap, code, "An error was discovered processing the <wsse:Security> header", detail);
    case wsse__InvalidSecurityToken:
      return soap_wsse_sender_fault_subcode(soap, code, "An invalid security token was provided", detail);
    case wsse__FailedAuthentication:
      return soap_wsse_sender_fault_subcode(soap, code, "The security token could not be authenticated or authorized", detail);
    case wsse__FailedCheck:
      return soap_wsse_sender_fault_subcode(soap, code, "The signature or decryption was invalid", detail);
    case wsse__SecurityTokenUnavailable:
      return soap_wsse_sender_fault_subcode(soap, code, "Referenced security token could not be retrieved", detail);
  }
  return SOAP_FAULT;
}

/**
@fn void soap_wsse_delete_Security(struct soap *soap)
@brief Deletes Security header element.
@param soap context
*/
void
soap_wsse_delete_Security(struct soap *soap)
{ DBGFUN("soap_wsse_delete_Security");
  if (soap->header)
    soap->header->wsse__Security = NULL;
}

/**
@fn int soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail)
@brief Sets sender SOAP Fault (sub)code for server fault response.
@param soap context
@param[in] faultsubcode sub code string
@param[in] faultstring fault string
@param[in] faultdetail detail string
@return SOAP_FAULT
*/
int
soap_wsse_sender_fault_subcode(struct soap *soap, const char *faultsubcode, const char *faultstring, const char *faultdetail)
{
#if defined(SOAP_WSA_2003) || defined(SOAP_WSA_2004) || defined(SOAP_WSA_200408) || defined(SOAP_WSA_2005)
  return soap_wsa_sender_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
#else
  return soap_sender_fault_subcode(soap, faultsubcode, faultstring, faultdetail);
#endif
}

#ifdef __cplusplus
}
#endif

