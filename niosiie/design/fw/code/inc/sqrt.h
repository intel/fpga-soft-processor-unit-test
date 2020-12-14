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
 * @file sqrt.h
 * @brief Functions that calculate square root with the ALTSQRT IP in the system
 */

#ifndef INC_SQRT_H
#define INC_SQRT_H

// Includes
#include "nios_sys.h"

// CSR Interface (refer to altsqrt_avmm_bridge.sv)
// Word Address           | Description
//----------------------------------------------------------------------
// 0x00                   | Radical (input to the ALTSQRT IP)
//----------------------------------------------------------------------
// 0x01                   | Q (output from the ALTSQRT IP)
//----------------------------------------------------------------------
// 0x02                   | Remainder (output from the ALTSQRT IP)
#define ALTSQRT_CSR_RADICAL_OFST 0
#define ALTSQRT_CSR_Q_OFST 1
#define ALTSQRT_CSR_REMAINDER_OFST 2

// input (i.e. the radical) of ALTSQRT_IP is only 8-bit wide.
#define ALTSQRT_RADICAL_BITMASK 0xFF

// The ALTSQRT_IP is configured to have 4-bit Q and 5-bit remainder as output
#define ALTSQRT_Q_BITMASK 0b1111
#define ALTSQRT_REMAINDER_BITMASK 0b11111

// helper functions
void calculate_sqrt(alt_u32 radical);
alt_u32 get_sqrt_q();
alt_u32 get_sqrt_remainder();

#endif  // INC_SQRT_H
