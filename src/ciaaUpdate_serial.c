/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
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

/** \brief This file implements the Flash Update serial transport layer
 **
 ** This file implements the Flash Update serial transport layer. It should
 ** be used for debug proposes only.
 **
 ** \todo change the serial struct. add a void * field to the transport struct.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Serial
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
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_errno.h"
#include "ciaaUpdate_transport.h"

/*==================[macros and definitions]=================================*/
typedef struct
{
   ciaaUpdate_transportRecv recv;
   ciaaUpdate_transportSend send;
   int32_t fd;
} ciaaUpdate_serialType;
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
ssize_t ciaaUpdate_serialSend(ciaaUpdate_transportType *serial, const void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != serial);

   return ciaaPOSIX_write(((ciaaUpdate_serialType *) serial)->fd, data, size);
}
ssize_t ciaaUpdate_serialRecv(ciaaUpdate_transportType *serial, void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != serial);

   return ciaaPOSIX_read(((ciaaUpdate_serialType *) serial)->fd, data, size);
}
/*==================[external functions definition]==========================*/
ciaaUpdate_transportType *ciaaUpdate_serialOpen(const char *dev)
{
   ciaaUpdate_serialType *serial;
   ciaaPOSIX_assert(NULL != dev);

   serial = ciaaPOSIX_malloc(sizeof(ciaaUpdate_serialType));

   serial->fd = ciaaPOSIX_open(dev, O_RDWR);
   ciaaPOSIX_assert(serial->fd >= 0);

   serial->recv = ciaaUpdate_serialRecv;
   serial->send = ciaaUpdate_serialSend;
   return (ciaaUpdate_transportType *) serial;
}
void ciaaUpdate_serialClose(ciaaUpdate_transportType *transport)
{
   ciaaUpdate_serialType *serial;
   ciaaPOSIX_assert(NULL != transport);

   serial = (ciaaUpdate_serialType *) transport;
   serial->send = NULL;
   serial->recv = NULL;
   ciaaPOSIX_close(serial->fd);
   serial->fd = -1;
   ciaaPOSIX_free(serial);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

