
#**************************************************************
# Create Clock
#**************************************************************
create_clock -name {sys_clk_main_fpga} -period 40.000 [get_ports {sys_clk_main_fpga}]
create_clock -name {u_ram_clk2} -period 40.000 [get_ports {u_ram_clk2}]

#**************************************************************
# Delays 
#**************************************************************
set_input_delay -clock sys_clk_main_fpga 0 sys_clk_main_fpga
set_input_delay -clock sys_clk_main_fpga 0 sys_reset

set_input_delay -clock sys_clk_main_fpga 0 u_gpi_data_proc_request
set_input_delay -clock sys_clk_main_fpga 0 u_gpi_clear_nios_state
set_input_delay -clock sys_clk_main_fpga 0 u_gpi_ping_request
set_input_delay -clock sys_clk_main_fpga 0 u_gpi_clear_ping_response
set_output_delay -clock sys_clk_main_fpga 0 u_gpo_nios_busy
set_output_delay -clock sys_clk_main_fpga 0 u_gpo_nios_done
set_output_delay -clock sys_clk_main_fpga 0 u_gpo_nios_error
set_output_delay -clock sys_clk_main_fpga 0 u_gpo_ping_response

set_input_delay -clock u_ram_clk2 0 u_ram_s2_address[*]
set_input_delay -clock u_ram_clk2 0 u_ram_s2_chipselect
set_input_delay -clock u_ram_clk2 0 u_ram_s2_clken
set_input_delay -clock u_ram_clk2 0 u_ram_s2_write
set_output_delay -clock u_ram_clk2 0 u_ram_s2_readdata[*]
set_input_delay -clock u_ram_clk2 0 u_ram_s2_writedata[*]
set_input_delay -clock u_ram_clk2 0 u_ram_s2_byteenable[*]
set_input_delay -clock u_ram_clk2 0 u_ram_clk2
set_input_delay -clock u_ram_clk2 0 u_ram_reset2_reset
set_input_delay -clock u_ram_clk2 0 u_ram_reset2_reset_req
