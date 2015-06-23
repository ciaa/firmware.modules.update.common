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

#ifndef UPDT_PACKER_H
#define UPDT_PACKER_H
/** \brief Flash Update Packer Header File
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Packer
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
#include "ciaaPOSIX_stdio.h"
#include "UPDT_IParser.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/** \brief Packer type */
typedef struct UPDT_packerType
{
   /** Parser reference */
   UPDT_parserType *parser;
   /** Buffer where the packed data will be stored */
   uint8_t *buffer;
   /** Pointer to the current segment data */
   const uint8_t *segment_data;
   /** Current segment size */
   uint32_t segment_size;
   /** Maximum buffer size */
   size_t buffer_max_size;
} UPDT_packerType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Writes the 8 bytes segment header into a buffer.
 **
 ** \param dest Destination buffer. Must have at least 8 bytes.
 ** \param segment_address Segment address.
 ** \param segment_size Segment size.
 **/
void UPDT_packerMakeHeader(uint8_t *dest, uint32_t segment_address, uint32_t segment_size);

/** \brief Writes the segment padding into a buffer.
 **
 ** This function creates a random padding of length n, such that
 ** n + segment_size is a multiple of 8.
 **
 ** \param dest Destination buffer.
 ** \param segment_size Segment size.
 ** \return The generated padding size.
 **/
uint8_t UPDT_packerMakePadding(uint8_t *dest, uint32_t segment_size);

/** \brief Initializes a packer structure.
 **
 ** \param packer Pointer to the structure.
 ** \param parse Callback to the parser function.
 ** \param buffer Pointer to the buffer.
 ** \param size Buffer size.
 ** \return 0 on success. Non-zero on error. */
int32_t UPDT_packerInit(
   UPDT_packerType *packer,
   UPDT_parserType *parser,
   uint8_t *buffer,
   size_t size);

/** \brief Gets a block of data using the parse function, packs it and stores
 ** it into the buffer. If there is enough data to parse the number of bytes
 ** packed is equal to the buffer size. Otherwise all the remaining data is
 ** packed.
 **
 ** \param packer Packer instance.
 ** \return number of bytes packed.
 **/
ssize_t UPDT_packerGet(UPDT_packerType *packer);

/** \brief Clears a packer structure.
 **
 ** \param packer Packer instance.
 **/
void UPDT_packerClear(UPDT_packerType *packer);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef UPDT_PACKER_H */
