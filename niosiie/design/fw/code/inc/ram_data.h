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
 * @file ram_data.h
 * @brief Contain struct map of the data in u_ram of the NIOS system.
 */

#ifndef INC_RAM_DATA_H
#define INC_RAM_DATA_H

// Includes
#include "nios_sys.h"

#define U_RAM_DATA_MAGIC 0x12345678
#define U_RAM_DATA_PTR ((RAM_DATA_T*) U_RAM_BASE)

/*!
 * Represent the data structure in u_ram.
 * This is meant as an example. This data doesn't exist.
 */
typedef struct
{
    alt_u32 magic;
    alt_u32 num1;
    alt_u32 num2;
    alt_u32 num3;
    alt_u32 data[U_RAM_SPAN - (sizeof(alt_u32) * 4)];
} RAM_DATA_T;

#endif  // INC_RAM_DATA_H
