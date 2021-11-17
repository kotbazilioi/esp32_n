/**
 * @file
 * Exports Private lwIP MIB 
 */

#ifndef LWIP_HDR_PRIVATE_MIB_H
#define LWIP_HDR_PRIVATE_MIB_H

#include "lwip/apps/snmp_opts.h"

#include "lwip/apps/snmp_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* export MIB */
extern const struct snmp_mib mib_private;
extern const struct snmp_mib mib_np_private;
extern const struct snmp_mib mib_termo_private;
void lwip_privmib_init(void);
void send_mess_trap (s32_t* OID_TR,char* mess,uint16_t lens_mess,uint8_t canal);
void send_mess_trap_termo (s32_t* OID_TR,uint8_t canal);
#ifdef __cplusplus
}
#endif

#endif
