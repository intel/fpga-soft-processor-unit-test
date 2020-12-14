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

#ifndef INC_SYSTEM_ONCHIP_MEM_MOCK_H
#define INC_SYSTEM_ONCHIP_MEM_MOCK_H

// Mock headers
#include "alt_types_mock.h"
#include "memory_mock.h"

// Macros
#define DEFAULT_ONCHIP_MEM_SIZE 4096

/**
 * Represent the onchip RAM (u_ram) in the Nios system.
 */
class ONCHIP_MEM_MOCK
{
public:
    static ONCHIP_MEM_MOCK* get();

    void reset();
    void resize_mem(alt_u32 nbytes);

    /**
     * @brief Return an x86 pointer to the mocked onchip memory space.
     */
    alt_u32* get_onchip_mem_ptr() { return m_onchip_mem; }

private:
    // Singleton inst
    static ONCHIP_MEM_MOCK* s_inst;

    ONCHIP_MEM_MOCK();
    ~ONCHIP_MEM_MOCK();

    // Memory mock for the onchip memory u_ram component
    alt_u32* m_onchip_mem = nullptr;
    alt_u32 m_onchip_mem_size = DEFAULT_ONCHIP_MEM_SIZE;
};

#endif /* INC_SYSTEM_ONCHIP_MEM_MOCK_H */
