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

module altsqrt_avmm_bridge #(
                             ) (
                                input wire          clk,
                                input wire          areset,

                                input wire [1:0]    avs_address,
                                input wire          avs_read,
                                input wire          avs_write,
                                output logic [31:0] avs_readdata,
                                input wire [31:0]   avs_writedata,

                                output logic [7:0]  altsqrt_radical, 
                                input wire [3:0]    altsqrt_q,
                                input wire [4:0]    altsqrt_remainder
                                );

    // CSR Interface
    // Word Address           | Description
    //----------------------------------------------------------------------
    // 0x00                   | Radical (input to the ALTSQRT IP)
    //----------------------------------------------------------------------
    // 0x01                   | Q (output from the ALTSQRT IP)
    //----------------------------------------------------------------------
    // 0x02                   | Remainder (output from the ALTSQRT IP)

    localparam CSR_RADICAL_ADDRESS = 2'd0;
    localparam CSR_Q_ADDRESS = 2'd1;
    localparam CSR_REMAINDER_ADDRESS = 2'd2;

    // Feed data into the ALTSQRT IP
    always_ff @(posedge clk)
    begin
        if (areset)
            altsqrt_radical <= 8'h0;
        else 
            if (avs_write && avs_address == CSR_RADICAL_ADDRESS) 
            begin
                altsqrt_radical <= avs_writedata[7:0];
            end 
    end 

    // Pass on the output of ALTSQRT IP
    always_comb 
    begin
        if (avs_read)
        begin
            avs_readdata = 32'h0;
            if (avs_address == CSR_Q_ADDRESS)
                avs_readdata = {28'h0, altsqrt_q};
            else if (avs_address == CSR_REMAINDER_ADDRESS)
                avs_readdata = {27'h0, altsqrt_remainder};
        end
        else
            avs_readdata = 32'h0;
    end

endmodule

