/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2012 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \brief SYSLOG implementation
 *
 * \author Daniele Basile <asterix@develer.com>
 *
 * notest:all
 */

#include "syslog.h"

#include "cfg_syslog.h"

//#include "byteorder.h" // host_to_net16

#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/netbuf.h>
#include <lwip/tcpip.h>
#include <lwip/api.h>
#include <stdarg.h>
#include <stdio.h>
#include "nvs_task.h"

#define kputs(x)  LWIP_DEBUGF( PING_DEBUG, (x))
static char syslog_message[CONFIG_SYSLOG_BUFSIZE];
 SysLog *local_syslog_ctx;
 SysLog *local_syslog_ctx1;
 SysLog *local_syslog_ctx2;
 ip4_addr_t ip4_syslog;

/**
 * Return the number of log message has been sent.
 */
uint32_t syslog_count(void)
{
	return local_syslog_ctx->syslog_cnt;
}

/**
 * Get the current syslog server address, in lwip ip_address format.
 */
ip_addr_t syslog_ip(void)
{
	return local_syslog_ctx->server_addr;
}

/**
 * Change the current syslog server ip address.
 * \param addr lwip ip_address (you could use the macro IP4_ADDR() to get it form ip address)
 */
void syslog_setIp(ip_addr_t addr)
{
	local_syslog_ctx->server_addr = addr;
}


/**
 * Print the log message on upd socket and serial if you configure the
 * macro CONFIG_SYSLOG_SERIAL in cfg_syslog.h,
 */
int syslog_printf(const char *fmt, ...)
{
  
 
             
   
  if ((FW_data.net.V_IP_SYSL[0]!=0)||(FW_data.net.V_IP_SYSL[1]!=0)||(FW_data.net.V_IP_SYSL[2]!=0)||(FW_data.net.V_IP_SYSL[3]!=0))
  {
    
    
        int len = sprintf(syslog_message,"%s %s %s", "<133>",FW_data.sys.V_Name_dev,fmt);
    
    
    
	syslog_message[sizeof(syslog_message) - 1] = 0;

	#if CONFIG_SYSLOG_SERIAL
		kputs(syslog_message);
	#endif

	if (local_syslog_ctx == NULL)
	{
		kputs("SysLog not init\n");
		return -1;
	}

	local_syslog_ctx->syslog_server = netconn_new(NETCONN_UDP);
	if (local_syslog_ctx->syslog_server == NULL)
	{
		kputs("Unable to alloc UDP connetions\n");
		return -1;
	}

	netbuf_ref(local_syslog_ctx->send_buf, syslog_message, len);
	if (netconn_sendto(local_syslog_ctx->syslog_server, local_syslog_ctx->send_buf,
						&(local_syslog_ctx->server_addr), CONFIG_SYSLOG_PORT) != ERR_OK)
	{
		kputs("Unable to send log!\n");
	}

	local_syslog_ctx->syslog_cnt++;
	netconn_delete(local_syslog_ctx->syslog_server);




	syslog_message[sizeof(syslog_message) - 1] = 0;

		#if CONFIG_SYSLOG_SERIAL
			kputs(syslog_message);
		#endif

		if (local_syslog_ctx1 == NULL)
		{
			kputs("SysLog not init\n");
			return -1;
		}

		local_syslog_ctx1->syslog_server = netconn_new(NETCONN_UDP);
		if (local_syslog_ctx1->syslog_server == NULL)
		{
			kputs("Unable to alloc UDP connetions\n");
			return -1;
		}

		netbuf_ref(local_syslog_ctx1->send_buf, syslog_message, len);
		if (netconn_sendto(local_syslog_ctx1->syslog_server, local_syslog_ctx1->send_buf,
							&(local_syslog_ctx1->server_addr), CONFIG_SYSLOG_PORT) != ERR_OK)
		{
			kputs("Unable to send log!\n");
		}

		local_syslog_ctx1->syslog_cnt++;
		netconn_delete(local_syslog_ctx1->syslog_server);



		syslog_message[sizeof(syslog_message) - 1] = 0;

			#if CONFIG_SYSLOG_SERIAL
				kputs(syslog_message);
			#endif

			if (local_syslog_ctx2 == NULL)
			{
				kputs("SysLog not init\n");
				return -1;
			}

			local_syslog_ctx2->syslog_server = netconn_new(NETCONN_UDP);
			if (local_syslog_ctx2->syslog_server == NULL)
			{
				kputs("Unable to alloc UDP connetions\n");
				return -1;
			}

			netbuf_ref(local_syslog_ctx2->send_buf, syslog_message, len);
			if (netconn_sendto(local_syslog_ctx2->syslog_server, local_syslog_ctx2->send_buf,
								&(local_syslog_ctx2->server_addr), CONFIG_SYSLOG_PORT) != ERR_OK)
			{
				kputs("Unable to send log!\n");
			}

			local_syslog_ctx2->syslog_cnt++;
			netconn_delete(local_syslog_ctx2->syslog_server);




      //  vPortFree(fmt);
        return len;
        
  }
  else
  {
  return 1;
  }
	
}

/**
 * Init the syslog message.
 *
 * \param syslog_ctx syslog context
 * \param addr lwip ip_address (you could use the macro IP4_ADDR() to get it form ip address)
 */
void syslog_init(SysLog *syslog_ctx, ip_addr_t addr)

 {
     if ((FW_data.net.V_IP_SYSL[0]!=0)||(FW_data.net.V_IP_SYSL[1]!=0)||(FW_data.net.V_IP_SYSL[2]!=0)||(FW_data.net.V_IP_SYSL[3]!=0))
      {
        syslog_ctx=(SysLog*)pvPortMalloc(300);
        syslog_ctx->server_addr = addr;
        syslog_ctx->send_buf =netbuf_new();
        local_syslog_ctx = syslog_ctx;
      }
        
}
