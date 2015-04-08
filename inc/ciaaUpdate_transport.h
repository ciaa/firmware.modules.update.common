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

#ifndef _CIAAUPDATE_TRANSPORT_H_
#define _CIAAUPDATE_TRANSPORT_H_
/** \brief Flash Update Transport Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Transport
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Transport
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
#include "ciaaPOSIX_stddef.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
struct ciaaUpdate_transportStruct;
typedef struct ciaaUpdate_transportStruct ciaaUpdate_transportType;

typedef ssize_t (*ciaaUpdate_transportRecv)(ciaaUpdate_transportType* transport, void* data, size_t size);
typedef ssize_t (*ciaaUpdate_transportSend)(ciaaUpdate_transportType* transport, const void* data, size_t size);

typedef struct ciaaUpdate_transportStruct
{
   ciaaUpdate_transportRecv recv;
   ciaaUpdate_transportSend send;
} ciaaUpdate_transportType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAUPDATE_TRANSPORT_H_ */

