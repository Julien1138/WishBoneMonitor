----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART_wb
--
-- Description:     WishBone Wrapper for the UART component (includes FIFOs)
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
use ieee.math_real.all;

library UART_LIB;
use UART_LIB.UART_PACK.all;

entity UART_wb is
    generic
    (
        GlobalClockFrequency    : integer := 50e6;  -- design frequency
        BaudRate                : integer := 9600;
        DataBits                : integer := 8;
        Endianness              : std_logic := '0'; -- 0 => LSB first; 1 => MSB first
        ParityBit               : integer := 1;
        Parity                  : std_logic := '0'; -- 0 => even Parity; 1 => odd Parity
        StopBits                : integer := 2;
        BufferSize              : integer := 1024;
        WB_Data_Width           : integer := 32
    );
    port
    (
    -- WishBone interface
        wb_clk_i        : in std_logic;
        wb_rst_i        : in std_logic;
        
        wb_we_i         : in std_logic;
        wb_dat_i        : in std_logic_vector(WB_Data_Width - 1 downto 0);
        wb_dat_o        : out std_logic_vector(WB_Data_Width - 1 downto 0);
        wb_cyc_i        : in std_logic;
        wb_stb_i        : in std_logic;
        wb_ack_o        : out std_logic;
        wb_rty_o        : out std_logic;
        
        wb_wait_o       : out std_logic;    -- Available data waiting
        
    -- Serial interface :
        Rx              : in std_logic;
        Tx              : out std_logic
    );
end UART_wb;

architecture behavior of UART_wb is

    signal Input_FIFO_Write_en  : std_logic;
    signal Input_FIFO_Data_in   : std_logic_vector(DataBits - 1 downto 0);
    signal Input_FIFO_Read_en   : std_logic;
    signal Input_FIFO_Data_out  : std_logic_vector(DataBits - 1 downto 0);
    signal Input_FIFO_Empty     : std_logic;
    signal Input_FIFO_Full      : std_logic;
    signal Output_FIFO_Write_en : std_logic;
    signal Output_FIFO_Data_in  : std_logic_vector(DataBits - 1 downto 0);
    signal Output_FIFO_Read_en  : std_logic;
    signal Output_FIFO_Data_out : std_logic_vector(DataBits - 1 downto 0);
    signal Output_FIFO_Empty    : std_logic;
    signal Output_FIFO_Full     : std_logic;
    
    -- UART interface signals :
    signal SendData         : std_logic;
    signal DataToSend       : std_logic_vector(DataBits - 1 downto 0);
    signal SerializerBusy   : std_logic;
    signal ParityError      : std_logic;
    signal DataAvailable    : std_logic;
    signal ReceivedData     : std_logic_vector(DataBits - 1 downto 0);
    
    signal wb_ack_o_int     : std_logic;
    signal wb_rty_o_int     : std_logic;
    
begin

    -- UART Receive
    Input_FIFO_Write_en <= DataAvailable;
    Input_FIFO_Data_in <= ReceivedData;

    -- UART Send
    Output_FIFO_Read_en <= '1' when Output_FIFO_Empty = '0' and SerializerBusy = '0' and SendData = '0' else
                           '0';
    DataToSend <= Output_FIFO_Data_out;
    UART_send_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            SendData <= '0';
        elsif rising_edge(wb_clk_i) then
            SendData <= Output_FIFO_Read_en;
        end if;
    end process;

    -- WB_read
    Input_FIFO_Read_en <= '1' when wb_cyc_i = '1' and wb_stb_i = '1' and wb_ack_o_int = '0' and wb_rty_o_int = '0' and wb_we_i = '0' else
                          '0';
    wb_dat_o_generate : for i in 0 to WB_Data_Width-1 generate
        wb_dat_o_if1 : if i < DataBits generate
            wb_dat_o(i) <= Input_FIFO_Data_out(i);
        end generate;
        wb_dat_o_if2 : if i >= DataBits generate
            wb_dat_o(i) <= '0';
        end generate;
    end generate wb_dat_o_generate;

    -- WB_write
    Output_FIFO_Write_en <= '1' when wb_cyc_i = '1' and wb_stb_i = '1' and wb_ack_o_int = '0' and wb_rty_o_int = '0' and wb_we_i = '1' else
                            '0'; 
    Output_FIFO_Data_in <= wb_dat_i(DataBits - 1 downto 0);
    
    -- WishBone Acknowledgement process
    WishBone_Acknowledgement_process : process(wb_clk_i)
    begin
        if rising_edge(wb_clk_i) then
            if wb_rst_i = '1' then
                wb_ack_o_int <= '0';
                wb_rty_o_int <= '0';
            else
                wb_ack_o_int <= '0';
                wb_rty_o_int <= '0';
                if wb_cyc_i = '1' and wb_stb_i = '1' and wb_ack_o_int = '0' and wb_rty_o_int = '0' then
                    if Output_FIFO_Full = '0' then
                        wb_ack_o_int <= '1';
                    else
                        wb_rty_o_int <= '1';
                    end if;
                end if;
            end if;
        end if;
    end process;
    wb_ack_o <= wb_ack_o_int;
    wb_rty_o <= wb_rty_o_int;
    
    -- wb_wait_o
    wb_wait_o <= not Input_FIFO_Empty;
    
    input_FIFO : UART_FIFO
    generic map
    (
        FIFOSize => BufferSize,
        Data_Width => DataBits
    )
    port map
    (
        rst => wb_rst_i,
        clk => wb_clk_i,
        Write_en => Input_FIFO_Write_en,
        Data_in => Input_FIFO_Data_in,
        Read_en => Input_FIFO_Read_en,
        Data_out => Input_FIFO_Data_out,
        
        FIFO_Empty => Input_FIFO_Empty,
        FIFO_Full => Input_FIFO_Full
    );
    
    output_FIFO : UART_FIFO
    generic map
    (
        FIFOSize => BufferSize,
        Data_Width => DataBits
    )
    port map
    (
        rst => wb_rst_i,
        clk => wb_clk_i,
        Write_en => Output_FIFO_Write_en,
        Data_in => Output_FIFO_Data_in,
        Read_en => Output_FIFO_Read_en,
        Data_out => Output_FIFO_Data_out,
        
        FIFO_Empty => Output_FIFO_Empty,
        FIFO_Full => Output_FIFO_Full
    );

    UART_inst : UART
    generic map
    (
        GlobalClockFrequency => GlobalClockFrequency,
        SerializerBaudRate => BaudRate,
        SerializerDataBits => DataBits,
        SerializerEndianness => Endianness,
        SerializerParityBit => ParityBit,
        SerializerParity => Parity,
        SerializerStopBits => StopBits,
        DeserializerBaudRate => BaudRate,
        DeserializerDataBits => DataBits,
        DeserializerEndianness => Endianness,
        DeserializerParityBit => ParityBit,
        DeserializerParity => Parity,
        DeserializerStopBits => StopBits
    )
    port map
    (
        rst => wb_rst_i,
        clk => wb_clk_i,
        Rx => Rx,
        Tx => Tx,
        SendData => SendData,
        DataToSend => DataToSend,
        SerializerBusy => SerializerBusy,
        ParityError => ParityError,
        DataAvailable => DataAvailable,
        ReceivedData => ReceivedData
    );

end behavior;