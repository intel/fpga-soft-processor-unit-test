/******************************************************************************
 * Copyright (c) 2020 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

/**
 * @file gpio_utils.h
 * @brief Define GPIOs that are connected to this NIOS system.
 */

#ifndef INC_GPIO_UTILS_H
#define INC_GPIO_UTILS_H

// Includes
#include "nios_sys.h"

#include "ram_data.h"
#include "sqrt.h"
#include "utils.h"

/***************************************
 * Bit masks for the GPIO bits in use
 ***************************************/
// Request NIOS to process data in the dual-port RAM
#define SYS_GPI_DATA_PROC_REQUEST_MASK 0b1
// Request NIOS to clear its DONE or ERROR states
#define SYS_GPI_CLEAR_NIOS_STATE_MASK 0b10
// Request NIOS for a ping response in the GPO
#define SYS_GPI_PING_REQUEST_MASK 0b100
// Request NIOS to clear the ping response in the GPO
#define SYS_GPI_CLEAR_PING_RESPONSE_MASK 0b1000

// NIOS is busy processing data in the dual-port RAM
#define SYS_GPO_NIOS_BUSY_MASK 0b1
// NIOS is done with processing data in the dual-port RAM
#define SYS_GPO_NIOS_DONE_MASK 0b10
// Data in the dual-port RAM is problematic
#define SYS_GPO_NIOS_ERROR_MASK 0b100
// A ping response from NIOS
#define SYS_GPO_PING_RESPONSE_MASK 0b1000

// Get alt_u32* pointer to the GPIO base address
#define U_GPI_ADDR (alt_u32*) U_GPI_BASE
#define U_GPO_ADDR (alt_u32*) U_GPO_BASE

// Helper functions to respond to requests from GPIs
void gpio_ping_req_handler();
void gpio_data_proc_handler();

#endif  // INC_GPIO_UTILS_H
