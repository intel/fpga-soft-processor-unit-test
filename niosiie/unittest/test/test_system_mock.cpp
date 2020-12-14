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
#include "ut_nios_wrapper.h"

/*
 * Basic System Mock test
 */

TEST(SystemMockTest, get_returns_inst)
{
    EXPECT_NE(nullptr, SYSTEM_MOCK::get());
}

TEST(SystemMockTest, reset)
{
    SYSTEM_MOCK::get()->reset();
}

TEST(SystemMockTest, throw_internal_error)
{
    // Do throw instead of abort()
    SYSTEM_MOCK::get()->set_assert_to_throw();

    EXPECT_ANY_THROW(SYSTEM_INTERNAL_ERROR("this is a drill. "));

    EXPECT_NO_THROW(SYSTEM_ASSERT(1));
    EXPECT_ANY_THROW(SYSTEM_ASSERT(0));
}

/**
 * Out-of-range memory access (in System Mock) should trigger an internal error.
 */
TEST(SystemMockTest, invalid_memory_access)
{
    // Do throw instead of abort()
    SYSTEM_MOCK::get()->set_assert_to_throw();

    alt_u32* invalid_mem_addr = (alt_u32*) 0;
    EXPECT_FALSE(SYSTEM_MOCK::get()->is_addr_in_range(invalid_mem_addr));
    EXPECT_ANY_THROW(IORD(invalid_mem_addr, 0));
}

/**
 * Check ALTSQRT MOCK implementation with a range of different inputs.
 */
TEST(SystemMockTest, check_altsqrt_mock_calc)
{
    // Check square root of 0
    IOWR(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_RADICAL_OFST, 0);
    alt_u32 res_q = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_Q_OFST);
    EXPECT_EQ(res_q, alt_u32(0));

    // Check square root of 4
    IOWR(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_RADICAL_OFST, 4);
    res_q = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_Q_OFST);
    EXPECT_EQ(res_q, alt_u32(2));

    // Check square root of 10
    IOWR(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_RADICAL_OFST, 10);
    res_q = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_Q_OFST);
    alt_u32 res_remainder = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_REMAINDER_OFST);
    EXPECT_EQ(res_q, alt_u32(3));
    EXPECT_EQ(res_remainder, alt_u32(1));

    // Check square root of 230
    IOWR(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_RADICAL_OFST, 230);
    res_q = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_Q_OFST);
    res_remainder = IORD(U_ALTSQRT_AVMM_BRIDGE_BASE, ALTSQRT_CSR_REMAINDER_OFST);
    EXPECT_EQ(res_q, alt_u32(15));
    EXPECT_EQ(res_remainder, alt_u32(5));
}
