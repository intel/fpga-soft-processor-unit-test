// Copyright (c) 2020 Intel Corporation

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

`timescale 1 ps / 1 ps
`default_nettype none

module top (
            input wire         sys_clk_main_fpga,
            input wire         sys_reset,
            // GPI connectivity from Nios
            input wire         u_gpi_data_proc_request,
            input wire         u_gpi_clear_nios_state,
            input wire         u_gpi_ping_request,
            input wire         u_gpi_clear_ping_response,
            // GPO connection from Nios
            output wire        u_gpo_nios_busy, 
            output wire        u_gpo_nios_done, 
            output wire        u_gpo_nios_error, 
            output wire        u_gpo_ping_response, 
            // Export connectivity to the dual-port RAM
            input wire [9:0]   u_ram_s2_address,
            input wire         u_ram_s2_chipselect,
            input wire         u_ram_s2_clken,
            input wire         u_ram_s2_write,
            output wire [31:0] u_ram_s2_readdata,
            input wire [31:0]  u_ram_s2_writedata,
            input wire [3:0]   u_ram_s2_byteenable,
            input wire         u_ram_clk2,
            input wire         u_ram_reset2_reset,
            input wire         u_ram_reset2_reset_req
            );

    // Nios general purpose inputs/outputs	
    wire [7:0]                 u_gpi;
    wire [7:0]                 u_gpo;

    ///////////////////////////////////////////////////////////////////////////
    // Nios IIe system
    ///////////////////////////////////////////////////////////////////////////
    top_sys u0 (
                .clk_clk                          (sys_clk_main_fpga),
                .reset_reset                      (sys_reset),
                .u_gpi_external_connection_export (u_gpi),
                .u_gpo_external_connection_export (u_gpo),
                .u_ram_s2_address                 (u_ram_s2_address),
                .u_ram_s2_chipselect              (u_ram_s2_chipselect),
                .u_ram_s2_clken                   (u_ram_s2_clken),
                .u_ram_s2_write                   (u_ram_s2_write),
                .u_ram_s2_readdata                (u_ram_s2_readdata),
                .u_ram_s2_writedata               (u_ram_s2_writedata),
                .u_ram_s2_byteenable              (u_ram_s2_byteenable),
                .u_ram_clk2_clk                   (u_ram_clk2),
                .u_ram_reset2_reset               (u_ram_reset2_reset),
                .u_ram_reset2_reset_req           (u_ram_reset2_reset_req)
                );

    ///////////////////////////////////////////////////////////////////////////
    // GPI Connectivity from Nios	
    ///////////////////////////////////////////////////////////////////////////
    // Request Nios to process data in the dual-port RAM
    assign u_gpi[0] = u_gpi_data_proc_request;
    // Request Nios to clear its DONE or ERROR states
    assign u_gpi[1] = u_gpi_clear_nios_state;
    // Request Nios for a ping response in the GPO
    assign u_gpi[2] = u_gpi_ping_request;
    // Request Nios to clear the ping response in the GPO
    assign u_gpi[3] = u_gpi_clear_ping_response;
    assign u_gpi[7:4] = 4'h0;

    ///////////////////////////////////////////////////////////////////////////
    // GPO Connectivity from Nios	
    ///////////////////////////////////////////////////////////////////////////
    // Nios is busy processing data in the dual-port RAM
    assign u_gpo_nios_busy = u_gpo[0];
    // Nios is done with processing data in the dual-port RAM
    assign u_gpo_nios_done = u_gpo[1];
    // Nios sees unexpected data in the dual-port RAM
    assign u_gpo_nios_error = u_gpo[2];
    // The ping response from Nios
    assign u_gpo_ping_response = u_gpo[3];

endmodule
