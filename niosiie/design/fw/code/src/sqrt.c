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
 * @file sqrt.c
 * @brief Functions that calculate square root with the ALTSQRT IP in the system
 */

// Includes
#include "sqrt.h"

/**
 * @brief Write the @p radical to the ALTSQRT CSR interface
 *
 * @param radical a number (only lowest 8 bits are used) as the radical of the square root
 */
void calculate_sqrt(alt_u32 radical)
{
    IOWR(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_RADICAL_OFST, radical & ALTSQRT_RADICAL_BITMASK);
}

/**
 * @brief Return the q number from the ALTSQRT IP.
 */
alt_u32 get_sqrt_q()
{
    return (IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_Q_OFST) & ALTSQRT_Q_BITMASK);
}

/**
 * @brief Return the remainder from the ALTSQRT IP.
 */
alt_u32 get_sqrt_remainder()
{
    return (IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_REMAINDER_OFST) &
            ALTSQRT_REMAINDER_BITMASK);
}
