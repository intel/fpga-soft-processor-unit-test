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
 * @file utils.c
 * @brief Commonly-used functions
 */

// Includes
#include "utils.h"

/**
 * @brief Set bit(s), according to the @p bitmask, in the target word.
 */
void set_bits(alt_u32* addr, alt_u32 bitmask)
{
    alt_u32 word = IORD(addr, 0);
    word |= bitmask;
    IOWR(addr, 0, word);
}

/**
 * @brief Clear bit(s),  according to the @p bitmask, in the target word.
 */
void clear_bits(alt_u32* addr, alt_u32 bitmask)
{
    alt_u32 word = IORD(addr, 0);
    word &= ~bitmask;
    IOWR(addr, 0, word);
}

/**
 * @brief Return 1 if any of the bits, specified in @p bitmask, is set
 * to 1 in the target word. Otherwise, return 0.
 */
alt_u32 check_bits(alt_u32* addr, alt_u32 bitmask)
{
    alt_u32 word = IORD(addr, 0);

    // If any of the bits in bitmask is set to 1 in word, return 1.
    if (word & bitmask)
    {
        return 1;
    }

    return 0;
}
