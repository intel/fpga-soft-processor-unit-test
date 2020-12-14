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

// include
#include "onchip_mem_mock.h"

// Static data
ONCHIP_MEM_MOCK* ONCHIP_MEM_MOCK::s_inst = nullptr;

// Return the singleton instance of spi flash mock
ONCHIP_MEM_MOCK* ONCHIP_MEM_MOCK::get()
{
    if (s_inst == nullptr)
    {
        s_inst = new ONCHIP_MEM_MOCK();
    }
    return s_inst;
}

// Constructor/Destructor
ONCHIP_MEM_MOCK::ONCHIP_MEM_MOCK() : m_onchip_mem_size(DEFAULT_ONCHIP_MEM_SIZE)
{
    m_onchip_mem = new alt_u32[m_onchip_mem_size / sizeof(alt_u32)];
}
ONCHIP_MEM_MOCK::~ONCHIP_MEM_MOCK()
{
    delete[] m_onchip_mem;
}

// Class methods
/**
 * Clear the memory mock and set all words to 0.
 */
void ONCHIP_MEM_MOCK::reset()
{
    // Erase the onchip memory
    for (alt_u32 i = 0; i < (m_onchip_mem_size / sizeof(alt_u32)); i++)
    {
        m_onchip_mem[i] = 0;
    }
}

/**
 * Resize the allocated memory for the memory mock.
 */
void ONCHIP_MEM_MOCK::resize_mem(alt_u32 nbytes)
{
    // Reallocate space for the memory mock
    delete[] m_onchip_mem;
    m_onchip_mem_size = nbytes;
    m_onchip_mem = new alt_u32[m_onchip_mem_size / sizeof(alt_u32)];
}
