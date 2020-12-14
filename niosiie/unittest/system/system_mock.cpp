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

// Standard headers
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/stacktrace.hpp>

// Mock headers
#include "system_mock.h"
#include "array_memory_mock.h"
#include "altsqrt_mock.h"

// Static data
SYSTEM_MOCK* SYSTEM_MOCK::s_inst = nullptr;

// Type definitions

// Constructor/Destructor
SYSTEM_MOCK::SYSTEM_MOCK() : m_assert_abort_or_throw(true)
{
    // Instantiate an onchip memory mock object that represents the u_ram component in the system.
    m_onchip_mem_mock_inst = ONCHIP_MEM_MOCK::get();
    m_onchip_mem_mock_inst->resize_mem(U_RAM_SPAN);

    // Make mock for the ALTSQRT AVMM bridge + IP
    m_memory_mocks.push_back(std::make_unique<ALTSQRT_MOCK>());

    // Make GPIO mocks (with generic array memory mock)
    m_memory_mocks.push_back(std::make_unique<ARRAY_MEMORY_MOCK<U_GPI_BASE, U_GPI_SPAN>>());
    m_memory_mocks.push_back(std::make_unique<ARRAY_MEMORY_MOCK<U_GPO_BASE, U_GPO_SPAN>>());

    // Make NIOS configuration RAM mocks
    m_memory_mocks.push_back(
        std::make_unique<ARRAY_MEMORY_MOCK<U_NIOS_CFG_RAM_BASE, U_NIOS_CFG_RAM_SPAN>>());
}

SYSTEM_MOCK::~SYSTEM_MOCK() {}

// Static methods
/**
 * Return the singleton object of System Mock.
 * If the System Mock object has not been instantiated, instantiate it and return the pointer.
 */
SYSTEM_MOCK* SYSTEM_MOCK::get()
{
    if (s_inst == nullptr)
    {
        s_inst = new SYSTEM_MOCK();
    }
    return s_inst;
}

// Class methods
/**
 * Return true if the given @p addr is in the memory range supported by System Mock.
 */
bool SYSTEM_MOCK::is_addr_in_range(void* addr)
{
    for (auto& mock : m_memory_mocks)
    {
        if (mock->is_addr_in_range(addr))
        {
            return true;
        }
    }
    return false;
}

/**
 * Reset all IP mocks and clear callback functions and enabled code blocks.
 */
void SYSTEM_MOCK::reset()
{
    // Clear all memory mocks
    for (auto& mock : m_memory_mocks)
    {
        mock->reset();
    }

    m_onchip_mem_mock_inst->reset();

    // Clear all RW callbacks
    m_read_write_callbacks.clear();

    // Clear all the inserted code blocks
    m_enabled_code_blocks.clear();
}

/**
 * Return the word at the target @p addr, if it's within the supported memory range.
 * Otherwise, raise an internal error.
 *
 * @param addr an address
 * @param nocallbacks if true, execute all the callback functions for memory READ
 */
alt_u32 SYSTEM_MOCK::get_mem_word(void* addr, bool nocallbacks)
{
    alt_u32 ret = 0;
    bool found_mock = false;

    if (!nocallbacks)
    {
        for (auto fn : m_read_write_callbacks)
        {
            fn(READ_OR_WRITE::READ, addr, ret);
        }
    }

    // Dispatch to the appropriate handler based on the address
    for (auto& mock : m_memory_mocks)
    {
        if (mock->is_addr_in_range(addr))
        {
            ret = mock->get_mem_word(addr);
            found_mock = true;
            break;
        }
    }

    if (!found_mock)
    {
        SYSTEM_INTERNAL_ERROR_VARG("Undefined handler for address %p", addr);
    }

    return ret;
}

/**
 * Set the word at the location @p addr to @p data, if the @p addr falls in the supported memory
 * range. Otherwise, raise an internal error.
 *
 * @param addr an address
 * @param data data to be written
 * @param nocallbacks if true, execute all the callback functions for memory WRITE
 */
void SYSTEM_MOCK::set_mem_word(void* addr, alt_u32 data, bool nocallbacks)
{
    bool found_mock = false;

    if (!nocallbacks)
    {
        for (auto fn : m_read_write_callbacks)
        {
            fn(READ_OR_WRITE::WRITE, addr, data);
        }
    }

    for (auto& mock : m_memory_mocks)
    {
        if (mock->is_addr_in_range(addr))
        {
            mock->set_mem_word(addr, data);
            found_mock = true;
            break;
        }
    }

    if (!found_mock)
    {
        SYSTEM_INTERNAL_ERROR_VARG("Undefined handler for address %p", addr);
    }
}

/**
 * Raise an internal error by throwing an exception or aborting the program, depending
 * on System Mock setting.
 */
void SYSTEM_MOCK::throw_internal_error(const std::string& msg, int line, const std::string& file)
{
    if (m_assert_abort_or_throw)
    {
        //        std::cerr << "Internal Error: " << msg << ", File: " << file << ",
        //        Line: " << line
        //        << std::endl; std::cerr << boost::stacktrace::stacktrace();
        std::cout << "Internal Error: " << msg << ", File: " << file << ", Line: " << line
                  << std::endl;
        std::cout << boost::stacktrace::stacktrace();
        std::abort();
    }
    else
    {
        throw - 1;
    }
}
