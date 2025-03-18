#ifndef __upnp_igd_cp_h__
#define __upnp_igd_cp_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform_adpt.h"

#define IGDCP_E_NO_GATEWAY	1	//caller has no gateway set
#define IGDCP_E_NO_UPNP_ROUTER	2
#define IGDCP_E_OP_FAILED	3


void UpnpIgdCpInitialize();

#define IGD_PORTTYPE_UDP	0
#define IGD_PORTTYPE_TCP	1
int UpnpIgdCpAddPortMap(const char *desc, unsigned short loc_port, unsigned short ext_port, int port_type);
void UpnpIgdCpDelPortMap(unsigned short ext_port, int port_type);

void UpnpIgdCpStop();

BOOL UpnpIgdCpGetNatMappedAddress(unsigned short loc_port, unsigned long *ext_ip, unsigned short *ext_port);

#ifdef __cplusplus
}
#endif

#endif

