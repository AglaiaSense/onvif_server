/* Because MD5 may not be implemented (at least, with the same
 * interface) on all systems, we have our own copy here.
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

#ifndef _SYS_MD5_H_
#define _SYS_MD5_H_

typedef unsigned UNSIGNED32;


/* MD5 context. */
typedef struct MD5Context {
  UNSIGNED32 state[4];	/* state (ABCD) */
  UNSIGNED32 count[2];	/* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];	/* input buffer */
} MD5_CTX;

void   our_MD5Init (MD5_CTX *);
void   our_MD5Update (MD5_CTX *, const unsigned char *, unsigned int);
void   our_MD5Pad (MD5_CTX *);
void   our_MD5Final (unsigned char [16], MD5_CTX *);
char * our_MD5End(MD5_CTX *, char *);
char * our_MD5File(const char *, char *);
char * our_MD5Data(const unsigned char *, unsigned int, char *);
#endif /* _SYS_MD5_H_ */

