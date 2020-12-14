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
 * @file nios_main.h
 * @brief Mainline function
 */

// Includes
#include "nios_main.h"

/**
 * @brief NIOS main() calls this function.
 */
int nios_main()
{
    // main loop
    while (1)
    {
        // Handle ping request
        gpio_ping_req_handler();

        // Handle request to process data in the dual-port onchip ram
        gpio_data_proc_handler();

#ifdef UNITTEST_MODE_ON
        // UNITTEST_MODE_ON is defined in the unittest compilation.
        if (SYSTEM_MOCK::get()->is_code_block_enabled(SYSTEM_MOCK::CODE_BLOCK::BREAK_LOOP))
        {
            break;
        }
#endif  // UNITTEST_MODE_ON
    }

    return 0;
}
