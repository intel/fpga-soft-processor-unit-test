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

#ifndef INC_SYSTEM_SYSTEM_MOCK_H
#define INC_SYSTEM_SYSTEM_MOCK_H

// Standard headers
#include <algorithm>
#include <functional>
#include <memory>
#include <string.h>
#include <string>
#include <vector>

// Include system.h from the NIOS BSP
#include "system.h"

// Mock headers
#include "alt_types_mock.h"
#include "memory_mock.h"
#include "onchip_mem_mock.h"

// Macros

// Nios firmware maps data on this on-chip ram with a struct.
// Replace this macro so that Nios firmware will interact to the x86 memory range instead.
#undef U_RAM_BASE
#define U_RAM_BASE SYSTEM_MOCK::get()->get_onchip_mem_ptr()

// Use Nios configuration RAM as a scratchpad for simple NIOS RW testing
#define NIOS_SCRATCHPAD_ADDR ((alt_u32*) U_NIOS_CFG_RAM_BASE)

// Define the asserts to use the system mock. This allows runtime control of
// abort() vs. throw
#define SYSTEM_ASSERT(condition)                                       \
    if (!(condition))                                                  \
    {                                                                  \
        SYSTEM_MOCK::get()->throw_internal_error(                      \
            std::string("Assert: ") + #condition, __LINE__, __FILE__); \
    }
#define SYSTEM_INTERNAL_ERROR(msg)                                         \
    {                                                                      \
        SYSTEM_MOCK::get()->throw_internal_error(msg, __LINE__, __FILE__); \
    }

#define SYSTEM_INTERNAL_ERROR_VARG(format, ...)                                      \
    {                                                                                \
        char buffer##__LINE__[4096];                                                 \
        ::snprintf(buffer##__LINE__, sizeof(buffer##__LINE__), format, __VA_ARGS__); \
        SYSTEM_INTERNAL_ERROR(buffer##__LINE__);                                     \
    }

// Class Definitions

/**
 * System Mock represents the entire hardware system in which the Nios system resides.
 * System Mock includes the mock modules for IPs that interact with the Nios system. In
 * this case, System Mock instantiates on-chip memory mock, GPIO mock and ALTSQRT IP
 * mock. It may also include external devices that are connected to the Nios system.
 */
class SYSTEM_MOCK
{
public:
    /**
     * Enum for read or write memory access, used in registering callback function.
     */
    enum class READ_OR_WRITE
    {
        READ,
        WRITE
    };

    /**
     * Names of code blocks in Nios firmware, which are only visible in unittest mode.
     */
    enum class CODE_BLOCK
    {
        BREAK_LOOP,
    };

    static SYSTEM_MOCK* get();
    void reset();

    // Support memory access
    bool is_addr_in_range(void* addr);
    alt_u32 get_mem_word(void* addr, bool nocallbacks = false);
    void set_mem_word(void* addr, alt_u32 data, bool nocallbacks = false);

    /**
     * Register a callback function, which will be executed during a memory read or write.
     */
    void register_read_write_callback(
        const std::function<void(READ_OR_WRITE read_or_write, void* addr, alt_u32 data)>& fn)
    {
        m_read_write_callbacks.push_back(fn);
    }

    /**
     * Return true if the code block @p code_blk is enabled.
     * There is a list of code blocks, written in Nios firmware files, that
     * are only compiled in during unittest compilation. By default, all
     * these code blocks are disabled and will not be executed.
     *
     * @see enable_code_block
     */
    bool is_code_block_enabled(CODE_BLOCK code_blk)
    {
        return (std::find(m_enabled_code_blocks.begin(), m_enabled_code_blocks.end(), code_blk) !=
                m_enabled_code_blocks.end());
    }
    /**
     * Allow the code block @p code_blk in Nios firmware to be executed.
     */
    void enable_code_block(CODE_BLOCK code_blk) { m_enabled_code_blocks.push_back(code_blk); }

    /**
     * Return the x86 pointer to the onchip RAM mock memory block.
     */
    alt_u32* get_onchip_mem_ptr() { return m_onchip_mem_mock_inst->get_onchip_mem_ptr(); }

    // Error handling
    void set_assert_to_throw() { m_assert_abort_or_throw = false; }
    void set_assert_to_abort() { m_assert_abort_or_throw = true; }
    void throw_internal_error(const std::string& msg, int line, const std::string& file);

private:
    // Singleton inst
    static SYSTEM_MOCK* s_inst;

    // Private constructor/destructor
    SYSTEM_MOCK();
    ~SYSTEM_MOCK();

    // Vector of memory mocks
    std::vector<std::unique_ptr<MEMORY_MOCK_IF>> m_memory_mocks;

    // Onchip memory mock
    ONCHIP_MEM_MOCK* m_onchip_mem_mock_inst = nullptr;

    // Callback functions
    std::vector<std::function<void(READ_OR_WRITE read_or_write, void* addr, alt_u32 data)>>
        m_read_write_callbacks;

    // list of enabled code block
    std::vector<CODE_BLOCK> m_enabled_code_blocks;

    // Control the behaviour of PFR_ASSERT
    bool m_assert_abort_or_throw;
};

#endif /* INC_SYSTEM_SYSTEM_MOCK_H_ */
