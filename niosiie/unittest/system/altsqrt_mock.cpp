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

// Include
#include <math.h>

#include "altsqrt_mock.h"

// Constructor/Destructor
ALTSQRT_MOCK::ALTSQRT_MOCK() {}
ALTSQRT_MOCK::~ALTSQRT_MOCK() {}

// Class methods

void ALTSQRT_MOCK::reset()
{
    m_altsqrt_csr_mem.reset();
}

bool ALTSQRT_MOCK::is_addr_in_range(void* addr)
{
    return m_altsqrt_csr_mem.is_addr_in_range(addr);
}

alt_u32 ALTSQRT_MOCK::get_mem_word(void* addr)
{
    return m_altsqrt_csr_mem.get_mem_word(addr);
}

void ALTSQRT_MOCK::set_mem_word(void* addr, alt_u32 data)
{
    std::uintptr_t addr_int = reinterpret_cast<std::uintptr_t>(addr);
    alt_u32 csr_offset = (addr_int - U_ALTSQRT_AVMM_BRIDGE_BASE) >> 2;

    // It's expected that NIOS only writes to the first offset, with the input "radical"
    if (csr_offset == 0)
    {
        // Calculate the square root with the input data as radical
        alt_u32 radical = data & 0xFF;
        alt_u32 q = static_cast<alt_u32>(sqrt(radical));
        alt_u32 remainder = radical - q * q;

        // Trim to supported number of bits
        q &= 0b1111;
        remainder &= 0b11111;

        // Save the output Q and Remainder at appropriate registers.
        alt_u32* altsqrt_csr_addr = (alt_u32*) U_ALTSQRT_AVMM_BRIDGE_BASE;
        m_altsqrt_csr_mem.set_mem_word(altsqrt_csr_addr + 1, q);
        m_altsqrt_csr_mem.set_mem_word(altsqrt_csr_addr + 2, remainder);
    }
}
