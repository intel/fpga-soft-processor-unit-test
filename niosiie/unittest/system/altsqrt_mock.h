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

#ifndef INC_ALTSQRT_MOCK_H
#define INC_ALTSQRT_MOCK_H

// Include system.h from the NIOS BSP
#include "system.h"
#include "unordered_map_memory_mock.h"

/**
 * Represent the ALT Square Root IP in the Nios system.
 */
class ALTSQRT_MOCK : public MEMORY_MOCK_IF
{
public:
    ALTSQRT_MOCK();
    virtual ~ALTSQRT_MOCK();

    void reset() override;

    alt_u32 get_mem_word(void* addr) override;
    void set_mem_word(void* addr, alt_u32 data) override;

    bool is_addr_in_range(void* addr) override;

private:
    // Memory range for ALTSQRT CSR interface
    UNORDERED_MAP_MEMORY_MOCK<U_ALTSQRT_AVMM_BRIDGE_BASE, U_ALTSQRT_AVMM_BRIDGE_SPAN>
        m_altsqrt_csr_mem;
};

#endif /* INC_ALTSQRT_MOCK_H */
