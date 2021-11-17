
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/components)
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

idf_component_register(SRCS 
"mime.c"
"sntp_task.c"
 "my_mib.c"
 "snmp_netconn.c"
 "snmp_core.c"
 "snmp_asn1.c" 
 "snmp_mib2.c" 
 "snmp_mib2_icmp.c" 
 "snmp_mib2_interfaces.c" 
 "snmp_mib2_ip.c"
 "snmp_mib2_snmp.c"
 "snmp_mib2_system.c"
 "snmp_mib2_tcp.c"
 "snmp_mib2_udp.c"
 "snmp_msg.c"
 "snmp_pbuf_stream.c"
# "snmp_raw.c"
 "snmp_scalar.c"
 "snmp_snmpv2_framework.c"
 "snmp_snmpv2_usm.c"
 "snmp_table.c"
"snmp_threadsync.c"
"snmp_traps.c"
"lwip_prvmib.c"
"sockets.c"

                    INCLUDE_DIRS ".")
                    
