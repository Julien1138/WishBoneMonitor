-----------------------------------------------------------------------------------
--
-- Project Name : wb_FIFO
-- Supplier     : Teuchos
--
-- Design Name  : wb_FIFO
-- Module Name  : wb_FIFO.vhd
--
-- Description  : Universal Asynchronous Receiver Transmitter
-- 
-- --------------------------------------------------------------------------------
-- Revision List
-- Version      Author(s)       Date        Changes
--
-- 1.0          J.Aupart        22/08/10    Creation
-----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

library wb_Memory_Lib;
use wb_Memory_Lib.wb_Memory_Pack.all;

entity wb_FIFO is
    generic
    (
        FIFOSize        : integer := 1024;
        WB_Data_Width   : integer := 8
    );
    port
    (
        wb_clk_i        : in std_logic;
        wb_rst_i        : in std_logic;
        
    -- Input Interface
        wb_we_i_Input   : in std_logic;
        wb_dat_i_Input  : in std_logic_vector(WB_Data_Width - 1 downto 0);
        wb_cyc_i_Input  : in std_logic;
        wb_stb_i_Input  : in std_logic;
        wb_ack_o_Input  : out std_logic;
        wb_ful_o_Input  : out std_logic;
        
    -- Output Interface
        wb_we_i_Output  : in std_logic;
        wb_dat_o_Output : out std_logic_vector(WB_Data_Width - 1 downto 0);
        wb_cyc_i_Output : in std_logic;
        wb_stb_i_Output : in std_logic;
        wb_ack_o_Output : out std_logic;
        wb_emp_o_Output : out std_logic
    );
end wb_FIFO;

architecture behavior of wb_FIFO is
    
    signal Write_en     : std_logic;
    signal Read_en      : std_logic;
    signal FIFO_Empty   : std_logic;
    signal FIFO_Full    : std_logic;
    
    signal wb_ack_o_Input_int   : std_logic;
    signal wb_ack_o_Output_int  : std_logic;
    
begin

    Input_Interface_Ack_process : process(wb_clk_i)
    begin
        if rising_edge(wb_clk_i) then
            if wb_rst_i = '1' then
                wb_ack_o_Input_int <= '0';
            else
                wb_ack_o_Input_int <= '0';
                if wb_cyc_i_Input = '1' and wb_stb_i_Input = '1' and wb_ack_o_Input_int = '0' then
                    wb_ack_o_Input_int <= '1';
                end if;
            end if;
        end if;
    end process;
    wb_ack_o_Input <= wb_ack_o_Input_int;

    Output_Interface_Ack_process : process(wb_clk_i)
    begin
        if rising_edge(wb_clk_i) then
            if wb_rst_i = '1' then
                wb_ack_o_Output_int <= '0';
            else
                wb_ack_o_Output_int <= '0';
                if wb_cyc_i_Output = '1' and wb_stb_i_Output = '1' and wb_ack_o_Output_int = '0' then
                    wb_ack_o_Output_int <= '1';
                end if;
            end if;
        end if;
    end process;
    wb_ack_o_Output <= wb_ack_o_Output_int;
    
    Write_en <= wb_cyc_i_Input and wb_stb_i_Input and wb_we_i_Input and (not wb_ack_o_Input_int) and (not FIFO_Full);
    Read_en <= wb_cyc_i_Output and wb_stb_i_Output and (not wb_we_i_Output) and (not wb_ack_o_Output_int) and (not FIFO_Empty);

    FIFO_inst : FIFO
    generic map
    (
        FIFOSize => FIFOSize,
        Data_Width => WB_Data_Width
    )
    port map
    (
        rst => wb_rst_i,
        clk => wb_clk_i,
        Write_en => Write_en,
        Data_in => wb_dat_i_Input,
        Read_en => Read_en,
        Data_out => wb_dat_o_Output,
        FIFO_Empty => FIFO_Empty,
        FIFO_Full => FIFO_Full
    );
    wb_emp_o_Output <= FIFO_Empty;
    wb_ful_o_Input <= FIFO_Full;

end behavior;