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

/**
 * This test suite exercises the main flow of the Nios firmware.
 */
class MainFlowTest : public testing::Test
{
public:
    // Get instance of system mock
    SYSTEM_MOCK* m_sys = SYSTEM_MOCK::get();

    virtual void SetUp()
    {
        // Reset system mock
        m_sys->reset();
    }

    virtual void TearDown() {}
};

TEST_F(MainFlowTest, test_happy_path)
{
    // Request to break from the main loop
    m_sys->enable_code_block(SYSTEM_MOCK::CODE_BLOCK::BREAK_LOOP);

    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());
}

TEST_F(MainFlowTest, test_ping_response)
{
    // Request to break from the main loop
    m_sys->enable_code_block(SYSTEM_MOCK::CODE_BLOCK::BREAK_LOOP);

    // Send in ping request
    IOWR(U_GPI_ADDR, 0, SYS_GPI_PING_REQUEST_MASK);
    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());
    // Check ping response
    EXPECT_EQ(IORD(U_GPO_ADDR, 0), alt_u32(SYS_GPO_PING_RESPONSE_MASK));

    // Send in clear request
    IOWR(U_GPI_ADDR, 0, SYS_GPI_CLEAR_PING_RESPONSE_MASK);
    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());
    // Check ping response (should have been cleared)
    EXPECT_EQ(IORD(U_GPO_ADDR, 0), alt_u32(0));
}

TEST_F(MainFlowTest, test_happy_path_with_onchip_mem_data)
{
    // Request to break from the main loop
    m_sys->enable_code_block(SYSTEM_MOCK::CODE_BLOCK::BREAK_LOOP);

    // Prepare some data in onchip memory
    RAM_DATA_T* ram_data = (RAM_DATA_T*) m_sys->get_onchip_mem_ptr();
    ram_data->magic = U_RAM_DATA_MAGIC;
    ram_data->num1 = 81;

    // Send in data processing request
    m_sys->register_read_write_callback(
        [](SYSTEM_MOCK::READ_OR_WRITE read_or_write, void* addr, alt_u32 data) {
            if (read_or_write == SYSTEM_MOCK::READ_OR_WRITE::READ)
            {
                // Send in the request, only when U_GPI_ADDR address is being read and the previous
                // states are cleared
                if (addr == U_GPI_ADDR)
                {
                    alt_u32 cur_gpo = SYSTEM_MOCK::get()->get_mem_word(U_GPO_ADDR, true);
                    if (!(cur_gpo & (SYS_GPO_NIOS_DONE_MASK | SYS_GPO_NIOS_ERROR_MASK)))
                    {
                        SYSTEM_MOCK::get()->set_mem_word(
                            U_GPI_ADDR, SYS_GPI_DATA_PROC_REQUEST_MASK, true);
                    }
                }
            }
        });

    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());

    // Check Nios status
    EXPECT_TRUE(check_bits(U_GPO_ADDR, SYS_GPO_NIOS_DONE_MASK));

    // Check sqrt calculation results
    EXPECT_EQ(ram_data->num2, alt_u32(9));
    EXPECT_EQ(ram_data->num3, alt_u32(0));

    // Send in clear Nios status request
    IOWR(U_GPI_ADDR, 0, SYS_GPI_CLEAR_NIOS_STATE_MASK);
    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());
    // Nios status should have been cleared
    EXPECT_EQ(IORD(U_GPO_ADDR, 0), alt_u32(0));
}

TEST_F(MainFlowTest, test_happy_path_with_bad_onchip_mem_data)
{
    // Request to break from the main loop
    m_sys->enable_code_block(SYSTEM_MOCK::CODE_BLOCK::BREAK_LOOP);

    // Prepare some data in onchip memory
    RAM_DATA_T* ram_data = (RAM_DATA_T*) m_sys->get_onchip_mem_ptr();
    ram_data->magic = U_RAM_DATA_MAGIC - 1;
    ram_data->num1 = 43;

    // Send in data processing request
    m_sys->register_read_write_callback(
        [](SYSTEM_MOCK::READ_OR_WRITE read_or_write, void* addr, alt_u32 data) {
            if (read_or_write == SYSTEM_MOCK::READ_OR_WRITE::READ)
            {
                // Send in the request, only when U_GPI_ADDR address is being read and the previous
                // states are cleared
                if (addr == U_GPI_ADDR)
                {
                    alt_u32 cur_gpo = SYSTEM_MOCK::get()->get_mem_word(U_GPO_ADDR, true);
                    if (!(cur_gpo & (SYS_GPO_NIOS_DONE_MASK | SYS_GPO_NIOS_ERROR_MASK)))
                    {
                        SYSTEM_MOCK::get()->set_mem_word(
                            U_GPI_ADDR, SYS_GPI_DATA_PROC_REQUEST_MASK, true);
                    }
                }
            }
        });

    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());

    // Check Nios status
    EXPECT_TRUE(check_bits(U_GPO_ADDR, SYS_GPO_NIOS_ERROR_MASK));

    // sqrt calculation should not be done, since the magic number is wrong.
    EXPECT_EQ(ram_data->num2, alt_u32(0));
    EXPECT_EQ(ram_data->num3, alt_u32(0));

    // Send in clear Nios status request
    IOWR(U_GPI_ADDR, 0, SYS_GPI_CLEAR_NIOS_STATE_MASK);
    // Run NIOS Main. Always run with the timeout
    ASSERT_DURATION_LE(10, nios_main());
    // Nios status should have been cleared
    EXPECT_EQ(IORD(U_GPO_ADDR, 0), alt_u32(0));
}
