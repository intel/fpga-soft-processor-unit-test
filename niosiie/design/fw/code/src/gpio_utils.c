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
 * @file gpio_utils.c
 * @brief Define utility functions that deal with GPIOs in the NIOS system
 */

// Includes
#include "gpio_utils.h"

/**
 * @brief Respond to ping request from external device.
 */
void gpio_ping_req_handler()
{
    if (check_bits(U_GPI_ADDR, SYS_GPI_CLEAR_PING_RESPONSE_MASK))
    {
        // Clear the ping response in GPO
        clear_bits(U_GPO_ADDR, SYS_GPO_PING_RESPONSE_MASK);
    }
    else if (check_bits(U_GPI_ADDR, SYS_GPI_PING_REQUEST_MASK))
    {
        // Send the ping response to GPO
        set_bits(U_GPO_ADDR, SYS_GPO_PING_RESPONSE_MASK);
    }
}

/**
 * @brief Handle data processing request from external device.
 * The external device is supposed to prepare some data in the dual-port onchip
 * RAM and issue a request through a GPI bit. In this case, the ask for NIOS
 * is to calculate the square root of a number in the given data. NIOS would save
 * the results at specific offsets within the RAM.
 *
 * NIOS uses GPIOs to perform handshake with external device. For example, if
 * the data is not in an expected format, NIOS will report an error through the GPOs.
 */
void gpio_data_proc_handler()
{
    if (check_bits(U_GPI_ADDR, SYS_GPI_DATA_PROC_REQUEST_MASK))
    {
        // There is a request to process the data in the dual-port onchip RAM
        // NIOS only acts on it when the state (DONE or ERROR) from previous processing is cleared.
        if (!(check_bits(U_GPI_ADDR, SYS_GPO_NIOS_DONE_MASK | SYS_GPO_NIOS_ERROR_MASK)))
        {
            // Set busy state
            set_bits(U_GPO_ADDR, SYS_GPO_NIOS_BUSY_MASK);

            // Process data
            RAM_DATA_T* ram_data = U_RAM_DATA_PTR;
            // Check magic number before proceeding
            if (ram_data->magic == U_RAM_DATA_MAGIC)
            {
                // Do some arbitrary work with the data
                alt_u32 radical = ram_data->num1 & ALTSQRT_RADICAL_BITMASK;
                calculate_sqrt(radical);
                ram_data->num2 = get_sqrt_q();
                ram_data->num3 = get_sqrt_remainder();

                // Successfully processed the data
                set_bits(U_GPO_ADDR, SYS_GPO_NIOS_DONE_MASK);
            }
            else
            {
                // The data doesn't meet expected format (i.e. the magic number is off)
                set_bits(U_GPO_ADDR, SYS_GPO_NIOS_ERROR_MASK);
            }

            // Finished with data. Clear busy state.
            clear_bits(U_GPO_ADDR, SYS_GPO_NIOS_BUSY_MASK);
        }
    }
    else if (check_bits(U_GPI_ADDR, SYS_GPI_CLEAR_NIOS_STATE_MASK))
    {
        // This is the request to clear results from previous work
        clear_bits(U_GPO_ADDR, SYS_GPO_NIOS_DONE_MASK | SYS_GPO_NIOS_ERROR_MASK);
    }
}
