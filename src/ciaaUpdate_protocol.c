/* Copyright 2014, Daniel Cohen
 * Copyright 2014, Esteban Volentini
 * Copyright 2014, Matias Giori
 * Copyright 2014, Franco Salinas
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief This file implements the Flash Update Protocol functionality
 **
 ** This file implements the functionality of the Flash Update Protocol
 **
 ** \todo Add device identification support.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Protocol
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141010 v0.0.2  FS  first operating version
 * 20141010 v0.0.1  EV  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "ciaaUpdate_protocol.h"
#include "ciaaUpdate_transport.h"
#include "ciaaUpdate_utils.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int8_t ciaaUpdate_protocolGetHeaderVersion(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return (header[0] & 0x0F);
}
int8_t ciaaUpdate_protocolGetPacketType(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return (header[0] >> 4) & 0x0F;
}
uint16_t ciaaUpdate_protocolGetPayloadSize(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return  ((uint16_t) header[3]) << 3;
}
uint8_t ciaaUpdate_protocolGetSequenceNumber(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);

   return header[1];
}
void ciaaUpdate_protocolSetHeaderVersion(uint8_t *header, uint8_t version)
{
   ciaaPOSIX_assert(NULL != header);
   ciaaPOSIX_assert(version <= CIAAUPDATE_PROTOCOL_VERSION);

   header[0] = (header[0] & 0xF0) | (version & 0x0F);
}
void ciaaUpdate_protocolSetPacketType(uint8_t *header, uint8_t packet_type)
{
   ciaaPOSIX_assert(NULL != header);

   header[0] = (header[0] & 0x0F) | ((packet_type << 4) & 0xF0);
}
void ciaaUpdate_protocolSetPayloadSize(uint8_t *header, uint16_t payload_size)
{
   ciaaPOSIX_assert(NULL != header);
   /* payload must be size multiple of 8 and smaller than 2048 */
   ciaaPOSIX_assert(0 == (payload_size & 0xF807));

   header[3] = (uint8_t) (payload_size >> 3);
}
void ciaaUpdate_protocolSetSequenceNumber(uint8_t *header, uint8_t sequence_number)
{
   ciaaPOSIX_assert(NULL != header);

   header[1] = sequence_number;
}
int ciaaUpdate_protocolRecv(
   ciaaUpdate_transportType *transport,
   uint8_t *buffer,
   size_t size)
{
   ssize_t ret;
   size_t bytes_read = 0;

   ciaaPOSIX_assert(NULL != buffer);

   if(0 == size)
   {
      return 0;
   }
   /* read the specified number of bytes */
   while(bytes_read < size)
   {
      ret = transport->recv(transport, buffer + bytes_read, size - bytes_read);
      if(ret < 0)
      {
         return CIAAUPDATE_PROTOCOL_ERROR_TRANSPORT;
      }
      bytes_read += ret;
   }
   return CIAAUPDATE_PROTOCOL_ERROR_NONE;
}
#if 0
int ciaaUpdate_protocolSendData(
   ciaaUpdate_transportType *transport,
   uint8_t *data,
   uint8_t data_size,
   uint8_t sequence_number)
{
   uint8_t packet[CIAAUPDATE_PROTOCOL_PACKET_MAX_SIZE];

   ciaaUpdate_protocolSetHeaderVersion(packet, ciaaUpdate_protocol_version);
   ciaaUpdate_protocolSetPacketType(packet, CIAAUPDATE_PROTOCOL_PACKET_DATA);
   ciaaUpdate_protocolSetPayloadSize(packet, data_size);
   ciaaUpdate_protocolSetSequenceNumber(packet, sequence_number);

   ciaaPOSIX_memcpy(packet + CIAAUPDATE_PROTOCOL_HEADER_SIZE, data, data_size);
   return transport->send(data, data_size);

}
int32_t ciaaUpdate_protocolSend(
   ciaaUpdate_transportType* transport,
   ciaaUpdate_protocolLoadCallback load_cb)
{
   /* holds return values */
   int32_t ret;
   uint16_t payload_size;
   /* packet payload */
   uint8_t payload_buffer[CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE];
   /* packet header */
   uint8_t header[CIAAUPDATE_PROTOCOL_HEADER_SIZE];
   uint8_t sequence_number;

   /* initialize global variables */
   ciaaUpdate_protocol_transport = transport;
   ciaaUpdate_protocol_store_cb = load_cb;

   sequence_number = ciaaPOSIX_rand() % 256;

   /* TODO: send client identification */

   /* here the version should be defined after negotiation */
   ciaaUpdate_protocol_version = CIAAUPDATE_PROTOCOL_VERSION;

   /* here the version should be defined after negotiation */

   /* receive client data packets */

   /* reset data packet handler state */
   ciaaUpdate_protocolHandleDataPayload(NULL, 0);

   do
   {
      /* get data ready for transmission */
      data_size = load_cb(data, CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE);
      ciaaPOSIX_assert(data_size >= 0);
      ciaaPOSIX_assert_msg(data_size & 0x07 == 0, "data size must be multiple of 8");

      /* retransmission loop */
      do
      {
         /* send data packet */
         ret = ciaaUpdate_protocolSendData(header, data, sequence_number);
         if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
            return ret;

         /* wait for ACK */
         ret = ciaaUpdate_protocolRecvAck();
      }
      while(ret == CIAAUPDATE_PROTOCOL_ERROR_ACK_TIMEOUT);
   }
   while(data_size == CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE);


   /* TODO: receive and verify signature if necessary */
   return CIAAUPDATE_PROTOCOL_ERROR_NONE;
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

