/**
 * @file
 * Ping sender module
 *
 */

/*
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 */

/** 
 * This is an example of a "ping" sender (with raw API and socket API).
 * It can be used as a start point to maintain opened a network connection, or
 * like a network "watchdog" for your device.
 *
 */

#include "lwip/opt.h"

#include "lwipopts.h"

#if LWIP_RAW /* don't build if not configured for use in lwipopts.h */

#include "ping.h"

#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
//#include "timers.h"
#include "lwip/inet_chksum.h"
#include "../main/app.h"
#if PING_USE_SOCKETS
#include "sockets.h"
//#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
//#include "ip4_addr.h"
//#include "ip_addr.h"
#endif /* PING_USE_SOCKETS */
#include "../main/nvs_task.h"

/**
 * PING_DEBUG: Enable debugging for PING.
 */
#ifndef PING_DEBUG
#define PING_DEBUG     LWIP_DBG_ON
#endif

/** ping target - should be a "ip_addr_t" */
#ifndef PING_TARGET
#define PING_TARGET   (netif_default?netif_default->gw:ip_addr_any)
#endif

/** ping receive timeout - in milliseconds */
#ifndef PING_RCV_TIMEO
#define PING_RCV_TIMEO 1000
#endif

/** ping delay - in milliseconds */
#ifndef PING_DELAY
#define PING_DELAY     1000
#endif


#define PING_ID        0x0001
/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
#define PING_ID        0xAFAF
#endif

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE 32
#endif

/** ping result action - no default action */
#ifndef PING_RESULT
#define PING_RESULT(ping_ok)
#endif

/* ping variables */
static u16_t ping_seq_num;
icmp_cntrl_t ping_data;
icmp_cntrl_t ping_data1;
static u32_t ping_time;
static u32_t ping_time1;

#if !PING_USE_SOCKETS
static struct raw_pcb *ping_pcb;
#endif /* PING_USE_SOCKETS */
uint16_t ct_cn_a,ct_cn_b,ct_cn_c;
uint16_t ct_cn_a1,ct_cn_b1,ct_cn_c1;
uint8_t en_ping_a,en_ping_b,en_ping_c;
uint8_t en_ping_a1,en_ping_b1,en_ping_c1;
uint8_t ct_max_res=0;
uint8_t ct_max_res1=0;
uint8_t flag_delay_ping=0;
uint8_t flag_delay_ping1=0;




/** Prepare a echo ICMP request */
static void
ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len)
{
  size_t i;
  size_t data_len = len - sizeof(struct icmp_echo_hdr);

  ICMPH_TYPE_SET(iecho, ICMP_ECHO);
  ICMPH_CODE_SET(iecho, 0);
  iecho->chksum = 0;
  iecho->id     = PING_ID;
  iecho->seqno  = htons(++ping_seq_num);

  /* fill the additional data buffer with some data */
  for(i = 0; i < data_len; i++) {
    ((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i+0x60;
  }

  iecho->chksum = inet_chksum(iecho, len);
}

#if PING_USE_SOCKETS

/* Ping using the socket ip */
static err_t
ping_send(int s, ip4_addr_t *addr)
{
  int err;
  struct icmp_echo_hdr *iecho;
  struct sockaddr_in to;
  size_t ping_size = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;
  LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

  iecho = (struct icmp_echo_hdr *)mem_malloc((mem_size_t)ping_size);
  if (!iecho) {
    return ERR_MEM;
  }

  ping_prepare_echo(iecho, (u16_t)ping_size);

  to.sin_len = sizeof(to);
  to.sin_family = AF_INET;

 inet_addr_from_ip4addr(&to.sin_addr, addr);

  err = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr*)&to, sizeof(to));

  mem_free(iecho);

  return (err ? ERR_OK : ERR_VAL);
}

static uint32_t
ping_recv(int s)
{
  char buf[64];
  int fromlen, len;
  struct sockaddr_in from;
  struct ip_hdr *iphdr;
  struct icmp_echo_hdr *iecho;
  uint32_t delay_time=0;

  while((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) > 0) {
    if (len >= (int)(sizeof(struct ip_hdr)+sizeof(struct icmp_echo_hdr))) {
      ip4_addr_t fromaddr;
     inet_addr_to_ip4addr(&fromaddr, &from.sin_addr);
      LWIP_DEBUGF( PING_DEBUG, ("ping: recv "));
   //   ip_addr_debug_print(PING_DEBUG, &fromaddr);
      delay_time = sys_now() - ping_time;
      LWIP_DEBUGF( PING_DEBUG, (" %"U32_F" ms\n", (sys_now() - ping_time)));

      iphdr = (struct ip_hdr *)buf;
      iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));
      if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
        /* do some ping result processing */
        PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
        return delay_time;
//        PING_RESULT((ICMPH_TYPE(iecho) == ICMP_DUR));
//        return;
//        PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ECHO));
//        return;

      } else {
        LWIP_DEBUGF( PING_DEBUG, ("ping: drop\n"));
      }
    }
  }

  if (len == 0) {
    LWIP_DEBUGF( PING_DEBUG, ("ping: recv - %"U32_F" ms - timeout\n", (sys_now()-ping_time)));
  }

  /* do some ping result processing */
  PING_RESULT(0);
  return 0;
}

static void
ping_thread(void *arg)
{
  int s;
  int timeout = PING_RCV_TIMEO;
  ip4_addr_t ping_target;
//  uint32_t time_out_ping;

  LWIP_UNUSED_ARG(arg);

  if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
    return;
  }
timeout++;
  lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
 //sockets[0]->conn->recv_timeout=1000;
  while (1) {
  //    vTaskDelay(FW_data.V_TIME_RESEND_PING/3);
 if (FW_data.wdt[0].V_EN_WATCHDOG==1)
  {
  if ((FW_data.wdt[0].V_EN_WATCHDOG_CN_A==1)&&(en_ping_a==0))
    {
    do{
    IP4_ADDR(&ping_target,FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
  //    LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
 //     ip_addr_debug_print(PING_DEBUG, &ping_target);
  //    LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
 //     LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
   //   ip_addr_debug_print(PING_DEBUG, &ping_target);
    //  LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data.timeout[0]=sys_now()-ping_time;
    if (ping_data.timeout[0]<500)
      {
        ping_data.flag_err[0]=0;
        ct_cn_a=0;

      }
    else
      {
        if (ct_cn_a<FW_data.wdt[0].V_MAX_REPID_PING)
        {
          ct_cn_a++;
          vTaskDelay(FW_data.wdt[0].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_a=0;
          en_ping_a=1;
          ping_data.flag_err[0]=1;

        }

      }
    }while((ct_cn_a!=0)&&(ct_cn_a<FW_data.wdt[0].V_MAX_REPID_PING-1));
   }


   if ((FW_data.wdt[0].V_EN_WATCHDOG_CN_B==1)&&(en_ping_b==0))
  {
    do{
     IP4_ADDR(&ping_target,FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
    //  LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
    //  LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
    //  LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      //LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data.timeout[1]=sys_now()-ping_time;
    if (ping_data.timeout[1]<500)
      {
        ping_data.flag_err[1]=0;
        ct_cn_b=0;

      }
    else
      {
        if (ct_cn_b<FW_data.wdt[0].V_MAX_REPID_PING)
        {
          ct_cn_b++;
           vTaskDelay(FW_data.wdt[0].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_a=0;
          en_ping_b=1;
          ping_data.flag_err[1]=1;
        }


      }
    }while((ct_cn_b!=0)&&(ct_cn_b<FW_data.wdt[0].V_MAX_REPID_PING-1));
  }
 //  vTaskDelay(FW_data.V_TIME_RESEND_PING/3);
   if ((FW_data.wdt[0].V_EN_WATCHDOG_CN_C==1)&&(en_ping_c==0))
  {
    do{
      IP4_ADDR(&ping_target,FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data.timeout[2]=sys_now()-ping_time;
    if (ping_data.timeout[2]<500)
      {
        ping_data.flag_err[2]=0;
        ct_cn_c=0;

      }
    else
      {

       if (ct_cn_c<FW_data.wdt[0].V_MAX_REPID_PING)
        {
          ct_cn_c++;
          vTaskDelay(FW_data.wdt[0].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_c=0;
          en_ping_c=1;
          ping_data.flag_err[2]=1;
        }

      }
     }while((ct_cn_c!=0)&&(ct_cn_c<FW_data.wdt[0].V_MAX_REPID_PING-1));
  }


    }
// vTaskResume (LED_taskHandle);
   // if (flag_delay_ping!=0)
   // {
  if (FW_data.wdt[0].V_TYPE_LOGIC==0)
      {
       if ( (ping_data.flag_err[0]!=0)||(ping_data.flag_err[1]!=0)||(ping_data.flag_err[2]!=0))
        {

        vTaskDelay(1000*(FW_data.wdt[0].V_PAUSE_RESET_TO_REPID+FW_data.wdt[0].V_TIME_RESET_PULSE));
        flag_delay_ping=0;
        en_ping_a=0;
        en_ping_b=0;
        en_ping_c=0;
        ct_cn_a=0;
        ct_cn_b=0;
        ct_cn_c=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[0].V_T_SEND_PING);
      }
     }
  if (FW_data.wdt[0].V_TYPE_LOGIC==1)
      {
       if ( (ping_data.flag_err[0]!=0)&&(ping_data.flag_err[1]!=0)&&(ping_data.flag_err[2]!=0))
        {
          vTaskDelay(1000*(FW_data.wdt[0].V_PAUSE_RESET_TO_REPID+FW_data.wdt[0].V_TIME_RESET_PULSE));
        flag_delay_ping=0;
        en_ping_a=0;
        en_ping_b=0;
        en_ping_c=0;
        ct_cn_a=0;
        ct_cn_b=0;
        ct_cn_c=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[0].V_T_SEND_PING);
      }
      }
  if (FW_data.wdt[0].V_TYPE_LOGIC==2)
      {
       if ( ((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[1]!=0))||((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[2]!=0)))
        {
          vTaskDelay(1000*(FW_data.wdt[0].V_PAUSE_RESET_TO_REPID+FW_data.wdt[0].V_TIME_RESET_PULSE));
        flag_delay_ping=0;
        en_ping_a=0;
        en_ping_b=0;
        en_ping_c=0;
        ct_cn_a=0;
        ct_cn_b=0;
        ct_cn_c=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[0].V_T_SEND_PING);
      }
      }

  }
}

static void
ping_thread1(void *arg)
{
  int s;
 const int* timeout = PING_RCV_TIMEO;

  ip4_addr_t ping_target;
//  uint32_t time_out_ping1;

  LWIP_UNUSED_ARG(arg);



  if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
    return;
  }
//timeout++;
  lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,(const int *)timeout, sizeof(*timeout));


  while (1) {
  //    vTaskDelay(FW_data.V_TIME_RESEND_PING/3);
 if (FW_data.wdt[1].V_EN_WATCHDOG==1)
  {
  if ((FW_data.wdt[1].V_EN_WATCHDOG_CN_A==1)&&(en_ping_a1==0))
    {
    do{
    IP4_ADDR(&ping_target,FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[0],FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[1],FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[2],FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
  //    LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
 //     ip_addr_debug_print(PING_DEBUG, &ping_target);
  //    LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
 //     LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
   //   ip_addr_debug_print(PING_DEBUG, &ping_target);
    //  LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data1.timeout[0]=sys_now()-ping_time1;
    if (ping_data1.timeout[0]<500)
      {
        ping_data1.flag_err[0]=0;
        ct_cn_a1=0;

      }
    else
      {
        if (ct_cn_a1<FW_data.wdt[1].V_MAX_REPID_PING)
        {
          ct_cn_a1++;
          vTaskDelay(FW_data.wdt[1].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_a=0;
          en_ping_a1=1;
          ping_data1.flag_err[0]=1;

        }

      }
    }while((ct_cn_a1!=0)&&(ping_data1.flag_err[0]==0));//while((ct_cn_a1!=0)&&(ct_cn_a1<FW_data.wdt[1].V_MAX_REPID_PING));
   }


   if ((FW_data.wdt[1].V_EN_WATCHDOG_CN_B==1)&&(en_ping_b1==0))
  {
    do{
     IP4_ADDR(&ping_target,FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[0],FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[1],FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[2],FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
    //  LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
    //  LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
    //  LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      //LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data1.timeout[1]=sys_now()-ping_time;
    if (ping_data1.timeout[1]<500)
      {
        ping_data1.flag_err[1]=0;
        ct_cn_b1=0;

      }
    else
      {
        if (ct_cn_b1<FW_data.wdt[1].V_MAX_REPID_PING)
        {
          ct_cn_b1++;
           vTaskDelay(FW_data.wdt[1].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_a=0;
          en_ping_b1=1;
          ping_data1.flag_err[1]=1;
        }


      }
    }while((ct_cn_b1!=0)&&(ping_data1.flag_err[1]==0));
  }
 //  vTaskDelay(FW_data.V_TIME_RESEND_PING/3);
   if ((FW_data.wdt[1].V_EN_WATCHDOG_CN_C==1)&&(en_ping_c1==0))
  {
    do{
      IP4_ADDR(&ping_target,FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[0],FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[1],FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[2],FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[3]);
    if (ping_send(s, &ping_target) == ERR_OK) {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
    //  ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    ping_data1.timeout[2]=sys_now()-ping_time1;
    if (ping_data1.timeout[2]<500)
      {
        ping_data1.flag_err[2]=0;
        ct_cn_c1=0;

      }
    else
      {

       if (ct_cn_c1<FW_data.wdt[1].V_MAX_REPID_PING)
        {
          ct_cn_c1++;
          vTaskDelay(FW_data.wdt[1].V_TIME_RESEND_PING-500);
        }
        else
        {
         // ct_cn_c=0;
          en_ping_c1=1;
          ping_data1.flag_err[2]=1;
        }

      }
     }while((ct_cn_c1!=0)&&(ping_data1.flag_err[2]==0));
  }


    }
// vTaskResume (LED_taskHandle);
   // if (flag_delay_ping!=0)
   // {
  if (FW_data.wdt[1].V_TYPE_LOGIC==0)
      {
       if ( (ping_data1.flag_err[0]!=0)||(ping_data1.flag_err[1]!=0)||(ping_data1.flag_err[2]!=0))
        {

        vTaskDelay(1000*(FW_data.wdt[1].V_PAUSE_RESET_TO_REPID+FW_data.wdt[1].V_TIME_RESET_PULSE));
        flag_delay_ping1=0;
        en_ping_a1=0;
        en_ping_b1=0;
        en_ping_c1=0;
        ct_cn_a1=0;
        ct_cn_b1=0;
        ct_cn_c1=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[1].V_T_SEND_PING);
      }
     }
  if (FW_data.wdt[1].V_TYPE_LOGIC==1)
      {
       if ( (ping_data1.flag_err[0]!=0)&&(ping_data1.flag_err[1]!=0)&&(ping_data1.flag_err[2]!=0))
        {
          vTaskDelay(1000*(FW_data.wdt[1].V_PAUSE_RESET_TO_REPID+FW_data.wdt[1].V_TIME_RESET_PULSE));
        flag_delay_ping1=0;
        en_ping_a1=0;
        en_ping_b1=0;
        en_ping_c1=0;
        ct_cn_a1=0;
        ct_cn_b1=0;
        ct_cn_c1=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[1].V_T_SEND_PING);
      }
      }
  if (FW_data.wdt[1].V_TYPE_LOGIC==2)
      {
       if ( ((ping_data1.flag_err[0]!=0)&&(ping_data1.flag_err[1]!=0))||((ping_data1.flag_err[0]!=0)&&(ping_data1.flag_err[2]!=0)))
        {
          vTaskDelay(1000*(FW_data.wdt[1].V_PAUSE_RESET_TO_REPID+FW_data.wdt[1].V_TIME_RESET_PULSE));
        flag_delay_ping1=0;
        en_ping_a1=0;
        en_ping_b1=0;
        en_ping_c1=0;
        ct_cn_a1=0;
        ct_cn_b1=0;
        ct_cn_c1=0;

      }
      else
      {
         vTaskDelay(1000*FW_data.wdt[1].V_T_SEND_PING);
      }
      }

  }
}



#else /* PING_USE_SOCKETS */

/* Ping using the raw ip */
static u8_t
ping_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p, ip_addr_t *addr)
{
  struct icmp_echo_hdr *iecho;
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_ASSERT("p != NULL", p != NULL);

  if (pbuf_header( p, -PBUF_IP_HLEN)==0) {
    iecho = (struct icmp_echo_hdr *)p->payload;

    if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
      LWIP_DEBUGF( PING_DEBUG, ("ping: recv "));
      ip_addr_debug_print(PING_DEBUG, addr);
      LWIP_DEBUGF( PING_DEBUG, (" %"U32_F" ms\n", (sys_now()-ping_time)));

      /* do some ping result processing */
      PING_RESULT(1);
      pbuf_free(p);
      return 1; /* eat the packet */
    }
  }

  return 0; /* don't eat the packet */
}

static void
ping_send(struct raw_pcb *raw, ip_addr_t *addr)
{
  struct pbuf *p;
  struct icmp_echo_hdr *iecho;
  size_t ping_size = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;

  LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
  ip_addr_debug_print(PING_DEBUG, addr);
  LWIP_DEBUGF( PING_DEBUG, ("\n"));
  LWIP_ASSERT("ping_size <= 0xffff", ping_size <= 0xffff);

  p = pbuf_alloc(PBUF_IP, (u16_t)ping_size, PBUF_RAM);
  if (!p) {
    return;
  }
  if ((p->len == p->tot_len) && (p->next == NULL)) {
    iecho = (struct icmp_echo_hdr *)p->payload;

    ping_prepare_echo(iecho, (u16_t)ping_size);

    raw_sendto(raw, p, addr);
    ping_time = sys_now();
  }
  pbuf_free(p);
}

static void
ping_timeout(void *arg)
{
  struct raw_pcb *pcb = (struct raw_pcb*)arg;
  ip_addr_t ping_target = PING_TARGET;

  LWIP_ASSERT("ping_timeout: no pcb given!", pcb != NULL);

  ping_send(pcb, &ping_target);

  sys_timeout(PING_DELAY, ping_timeout, pcb);
}

static void
ping_raw_init(void)
{
  ping_pcb = raw_new(IP_PROTO_ICMP);
  LWIP_ASSERT("ping_pcb != NULL", ping_pcb != NULL);

  raw_recv(ping_pcb, ping_recv, NULL);
  raw_bind(ping_pcb, IP_ADDR_ANY);
  sys_timeout(PING_DELAY, ping_timeout, ping_pcb);
}

void
ping_send_now()
{
  ip_addr_t ping_target = PING_TARGET;
  LWIP_ASSERT("ping_pcb != NULL", ping_pcb != NULL);
  ping_send(ping_pcb, &ping_target);
}

#endif /* PING_USE_SOCKETS */

void
ping_init(void)
{
#if PING_USE_SOCKETS

  xTaskCreate(&ping_thread, "ping_thread", 1024, NULL, 10, NULL);
  xTaskCreate(&ping_thread1, "ping_thread1", 1024, NULL, 10, NULL);
#else /* PING_USE_SOCKETS */
  ping_raw_init();
#endif /* PING_USE_SOCKETS */
}

#endif /* LWIP_RAW */




void ct_res_wdt (void)
{
if (FW_data.wdt[0].V_CT_RES_ALLSTART<0xffff)
    {
    FW_data.wdt[0].V_CT_RES_ALLSTART++;
    }
    else
    {
    FW_data.wdt[0].V_CT_RES_ALLSTART=0;
    }
}

void ct_res_wdt1 (void)
{
if (FW_data.wdt[1].V_CT_RES_ALLSTART<0xffff)
    {
    FW_data.wdt[1].V_CT_RES_ALLSTART++;
    }
    else
    {
    FW_data.wdt[1].V_CT_RES_ALLSTART=0;
    }
}
