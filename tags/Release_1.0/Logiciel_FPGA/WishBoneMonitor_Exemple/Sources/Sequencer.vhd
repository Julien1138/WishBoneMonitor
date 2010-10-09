----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     Sequencer
--
-- Description:     Sequencer Main Sequencer
--
-- --------------------------------------------------------------------------------
-- Revision List
-- Version      Author(s)       Date        Changes
--
-- 1.0          J.Aupart        11/09/10    Creation
-----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

library MailBox_Lib;
use MailBox_Lib.MailBox_Pack.all;

entity Sequencer is
    generic
    (
        WB_Addr_Width   : integer := 4
    );
    port
    (
        wb_clk_i            : in std_logic;
        wb_rst_i            : in std_logic;
        
    -- UART interface
        wb_we_o_UART        : out std_logic;
        wb_dat_o_UART       : out std_logic_vector(7 downto 0);
        wb_dat_i_UART       : in std_logic_vector(7 downto 0);
        wb_cyc_o_UART       : out std_logic;
        wb_stb_o_UART       : out std_logic;
        wb_ack_i_UART       : in std_logic;
        wb_rty_i_UART       : in std_logic;
        wb_wait_i_UART      : in std_logic;
        
    -- MailBox interface
        wb_we_o_MailBox     : out std_logic;
        wb_adr_o_MailBox    : out std_logic_vector(WB_Addr_Width + 2 downto 0);
        wb_dat_o_MailBox    : out std_logic_vector(31 downto 0);
        wb_dat_i_MailBox    : in std_logic_vector(31 downto 0);
        wb_cyc_o_MailBox    : out std_logic;
        wb_stb_o_MailBox    : out std_logic;
        wb_ack_i_MailBox    : in std_logic;
        wb_dtr_i_MailBox    : in std_logic;
        wb_atr_i_MailBox    : in std_logic_vector(WB_Addr_Width downto 0);
        
        RTC_time            : in std_logic_vector(15 downto 0)
    );
end Sequencer;

architecture Sequencer_behavior of Sequencer is

    signal UART_read                : std_logic;
    signal UART_Rx_Frame            : std_logic_vector(79 downto 0);
    signal UART_Rx_Frame_Header     : std_logic_vector(7 downto 0);
    signal UART_Rx_Frame_Address    : std_logic_vector(15 downto 0);
    signal UART_Rx_Frame_Data       : std_logic_vector(31 downto 0);
    signal UART_Rx_Frame_Period     : std_logic_vector(15 downto 0);
    signal UART_Rx_Frame_CheckSum   : std_logic_vector(7 downto 0);
    signal UART_Rx_Frame_Valid      : std_logic;
    
    signal MailBox_write                : std_logic;
    signal MailBox_written              : std_logic;
    signal MailBox_write_Data           : std_logic;
    signal MailBox_write_Period         : std_logic;
    signal MailBox_write_Time           : std_logic;
    signal MailBox_write_Latch          : std_logic;
    signal MailBox_Direction_to_write   : std_logic;
    signal MailBox_Address_to_write     : std_logic_vector(WB_Addr_Width - 1 downto 0);
    signal MailBox_Data_to_write        : std_logic_vector(31 downto 0);
    signal MailBox_Period_to_write      : std_logic_vector(15 downto 0);
    
    signal MailBox_read             : std_logic;
    signal MailBox_read_Data        : std_logic;
    signal MailBox_read_Date        : std_logic;
    signal MailBox_read_Latch       : std_logic;
    signal MailBox_write_UART_Frame : std_logic;
    signal MailBox_Direction_read   : std_logic;
    signal MailBox_Address_read     : std_logic_vector(WB_Addr_Width - 1 downto 0);
    signal UART_Tx_Frame_Address    : std_logic_vector(15 downto 0);
    signal MailBox_Data_read        : std_logic_vector(31 downto 0);
    signal MailBox_Date_read        : std_logic_vector(15 downto 0);
    signal UART_Tx_Frame_CheckSum   : std_logic_vector(7 downto 0);
    
    signal UART_write           : std_logic;
    signal UART_Tx_Frame        : std_logic_vector(79 downto 0);
    signal UART_Tx_Byte_Counter : std_logic_vector(3 downto 0);

begin
    
--===========================================================================
--         Gestion des ports WishBone
--===========================================================================
    
    -- Interface UART
    UART_Interface_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            wb_we_o_UART <= '0';
            wb_cyc_o_UART <= '0';
            wb_stb_o_UART <= '0';
            wb_dat_o_UART <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if UART_read = '1' then
                if wb_ack_i_UART = '0' then
                    wb_we_o_UART <= '0';
                    wb_cyc_o_UART <= '1';
                    wb_stb_o_UART <= '1';
                else
                    wb_we_o_UART <= '0';
                    wb_cyc_o_UART <= '0';
                    wb_stb_o_UART <= '0';
                end if;
            elsif UART_write = '1' then
                if wb_ack_i_UART = '0' and wb_rty_i_UART = '0' then
                    wb_we_o_UART <= '1';
                    wb_cyc_o_UART <= '1';
                    wb_stb_o_UART <= '1';
                    wb_dat_o_UART <= UART_Tx_Frame(79 downto 72);
                else
                    wb_we_o_UART <= '0';
                    wb_cyc_o_UART <= '0';
                    wb_stb_o_UART <= '0';
                end if;
            else
                wb_we_o_UART <= '0';
                wb_cyc_o_UART <= '0';
                wb_stb_o_UART <= '0';
            end if;
        end if;
    end process;
    
    -- Interface MailBox
    MailBox_Interface_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            wb_we_o_MailBox <= '0';
            wb_cyc_o_MailBox <= '0';
            wb_stb_o_MailBox <= '0';
            wb_adr_o_MailBox <= (others => '0');
            wb_dat_o_MailBox <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_write_Data = '1' then
                if wb_ack_i_MailBox = '0' then
                    wb_we_o_MailBox <= '1';
                    wb_cyc_o_MailBox <= '1';
                    wb_stb_o_MailBox <= '1';
                    wb_adr_o_MailBox <= DataTable_Addr &
                                        MailBox_Direction_to_write &
                                        MailBox_Address_to_write;
                    wb_dat_o_MailBox <= MailBox_Data_to_write;
                else
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '0';
                    wb_stb_o_MailBox <= '0';
                end if;
            elsif MailBox_write_Period = '1' then
                if wb_ack_i_MailBox = '0' then
                    wb_we_o_MailBox <= '1';
                    wb_cyc_o_MailBox <= '1';
                    wb_stb_o_MailBox <= '1';
                    wb_adr_o_MailBox <= Recurrence_Addr &
                                        MailBox_Direction_to_write &
                                        MailBox_Address_to_write;
                    wb_dat_o_MailBox <= X"0000" & MailBox_Period_to_write;
                else
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '0';
                    wb_stb_o_MailBox <= '0';
                end if;
            elsif MailBox_write_Time = '1' then
                if wb_ack_i_MailBox = '0' then
                    wb_we_o_MailBox <= '1';
                    wb_cyc_o_MailBox <= '1';
                    wb_stb_o_MailBox <= '1';
                    wb_adr_o_MailBox <= Timetable_Addr &
                                        MailBox_Direction_to_write &
                                        MailBox_Address_to_write;
                    wb_dat_o_MailBox <= X"0000" & RTC_time;
                else
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '0';
                    wb_stb_o_MailBox <= '0';
                end if;
            elsif MailBox_read_Date = '1' then
                if wb_ack_i_MailBox = '0' then
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '1';
                    wb_stb_o_MailBox <= '1';
                    wb_adr_o_MailBox <= DatingTable_Addr &
                                        MailBox_Direction_read &
                                        MailBox_Address_read;
                else
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '0';
                    wb_stb_o_MailBox <= '0';
                end if;
            elsif MailBox_read_Data = '1' then
                if wb_ack_i_MailBox = '0' then
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '1';
                    wb_stb_o_MailBox <= '1';
                    wb_adr_o_MailBox <= DataTable_Addr &
                                        MailBox_Direction_read &
                                        MailBox_Address_read;
                else
                    wb_we_o_MailBox <= '0';
                    wb_cyc_o_MailBox <= '0';
                    wb_stb_o_MailBox <= '0';
                end if;
            else
                wb_we_o_MailBox <= '0';
                wb_cyc_o_MailBox <= '0';
                wb_stb_o_MailBox <= '0';
            end if;
        end if;
    end process;

    
--===========================================================================
--         Séquencement des opérations   
--===========================================================================
    
    Read_UART_Frame_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            UART_Rx_Frame <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if UART_read = '1' and wb_ack_i_UART = '1' then
                UART_Rx_Frame <= UART_Rx_Frame(71 downto 0) & wb_dat_i_UART;
            end if;
        end if;
    end process;
    UART_Rx_Frame_Header    <= UART_Rx_Frame(79 downto 72);
    UART_Rx_Frame_Address   <= UART_Rx_Frame(71 downto 56);
    UART_Rx_Frame_Data      <= UART_Rx_Frame(55 downto 24);
    UART_Rx_Frame_Period    <= UART_Rx_Frame(23 downto 8);
    UART_Rx_Frame_CheckSum <= UART_Rx_Frame( 7 downto  0) xor
                              UART_Rx_Frame(15 downto  8) xor
                              UART_Rx_Frame(23 downto 16) xor
                              UART_Rx_Frame(31 downto 24) xor
                              UART_Rx_Frame(39 downto 32) xor
                              UART_Rx_Frame(47 downto 40) xor
                              UART_Rx_Frame(55 downto 48) xor
                              UART_Rx_Frame(63 downto 56) xor
                              UART_Rx_Frame(71 downto 64) xor
                              UART_Rx_Frame(79 downto 72);
    UART_Rx_Frame_Valid <= '1' when UART_Rx_Frame_Header(7 downto 1) = "0000000" and UART_Rx_Frame_CheckSum = 0 else
                           '0';
                           
    MailBox_write_Latch_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            MailBox_Direction_to_write <= '0';
            MailBox_Address_to_write <= (others => '0');
            MailBox_Data_to_write <= (others => '0');
            MailBox_Period_to_write <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_write_Latch = '1' then
                MailBox_Address_to_write <= UART_Rx_Frame_Address(WB_Addr_Width - 1 downto 0);
                MailBox_Direction_to_write <= UART_Rx_Frame_Header(0);
                MailBox_Data_to_write <= UART_Rx_Frame_Data;
                MailBox_Period_to_write <= UART_Rx_Frame_Period;
            end if;
        end if;
    end process;
    
    MailBox_read_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            MailBox_Address_read <= (others => '0');
            MailBox_Direction_read <= '0';
            MailBox_Date_read <= (others => '0');
            MailBox_Data_read <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_read_Latch = '1' then
                MailBox_Address_read <= wb_atr_i_MailBox(WB_Addr_Width - 1 downto 0);
                MailBox_Direction_read <= wb_atr_i_MailBox(WB_Addr_Width);
            end if;
            if MailBox_read_Date = '1' and wb_ack_i_MailBox = '1' then
                MailBox_Date_read <= wb_dat_i_MailBox(15 downto 0);
            end if;
            if MailBox_read_Data = '1' and wb_ack_i_MailBox = '1' then
                MailBox_Data_read <= wb_dat_i_MailBox;
            end if;
        end if;
    end process;
    UART_Tx_Frame_CheckSum <= X"55" xor
                              UART_Tx_Frame_Address(15 downto  8) xor
                              UART_Tx_Frame_Address( 7 downto  0) xor
                              MailBox_Data_read(31 downto 24) xor
                              MailBox_Data_read(23 downto 16) xor
                              MailBox_Data_read(15 downto  8) xor
                              MailBox_Data_read( 7 downto  0) xor
                              MailBox_Date_read(15 downto  8) xor
                              MailBox_Date_read( 7 downto  0);
    
    UART_Tx_Frame_Address_generate : for i in 0 to 15 generate
        UART_Tx_Frame_Address_if1 : if i < WB_Addr_Width generate
            UART_Tx_Frame_Address(i) <= MailBox_Address_read(i);
        end generate;
        UART_Tx_Frame_Address_if2 : if i >= WB_Addr_Width generate
            UART_Tx_Frame_Address(i) <= '0';
        end generate;
    end generate;
    
    UART_Tx_Byte_Counter_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            UART_Tx_Byte_Counter <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if UART_write = '1' and wb_ack_i_UART = '1' then
                if UART_Tx_Byte_Counter = 9 then
                    UART_Tx_Byte_Counter <= (others => '0');
                else
                    UART_Tx_Byte_Counter <= UART_Tx_Byte_Counter + 1;
                end if;
            end if;
        end if;
    end process;
    
    Write_UART_Frame_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            UART_Tx_Frame <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_write_UART_Frame = '1' then
                UART_Tx_Frame <= X"55" &
                                 UART_Tx_Frame_Address &
                                 MailBox_Data_read &
                                 MailBox_Date_read &
                                 UART_Tx_Frame_CheckSum;
            end if;
            if UART_write = '1' and wb_ack_i_UART = '1' then
                UART_Tx_Frame <= UART_Tx_Frame(71 downto 0) & UART_Tx_Frame(79 downto 72);
            end if;
        end if;
    end process;

    Operation_sequencer_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            UART_read <= '0';
            MailBox_write <= '0';
            MailBox_written <= '1';
            MailBox_write_Data <= '0';
            MailBox_write_Period <= '0';
            MailBox_write_Time <= '0';
            MailBox_write_Latch <= '0';
            MailBox_read <= '0';
            MailBox_read_Data <= '0';
            MailBox_read_Date <= '0';
            MailBox_read_Latch <= '0';
            MailBox_write_UART_Frame <= '0';
            UART_write <= '0';
        elsif rising_edge(wb_clk_i) then
        
            if UART_read = '0' and wb_wait_i_UART = '1' then
                UART_read <= '1';
            end if;
            if UART_read = '1' and wb_ack_i_UART = '1' then
                UART_read <= '0';
            end if;
            
            if MailBox_write = '0' and MailBox_written = '0' and UART_Rx_Frame_Valid = '1' then
                MailBox_write <= '1';
                MailBox_write_Latch <= '1';
            end if;
            if MailBox_write_Latch = '1' then
                MailBox_write_Latch <= '0';
                MailBox_write_Data <= '1';
            end if;
            if MailBox_write_Data = '1' and wb_ack_i_MailBox = '1' then
                MailBox_write_Data <= '0';
                MailBox_write_Period <= '1';
            end if;
            if MailBox_write_Period = '1' and wb_ack_i_MailBox = '1' then
                MailBox_write_Period <= '0';
                MailBox_write_Time <= '1';
            end if;
            if MailBox_write_Time = '1' and wb_ack_i_MailBox = '1' then
                MailBox_write_Time <= '0';
                MailBox_written <= '1';
                MailBox_write <= '0';
            end if;
            if UART_Rx_Frame_Valid = '0' then
                MailBox_written <= '0';
            end if;
            
            if MailBox_read = '0' and wb_dtr_i_MailBox = '1' then
                MailBox_read <= '1';
                MailBox_read_Latch <= '1';
            end if;
            if MailBox_read_Latch = '1' then
                MailBox_read_Latch <= '0';
                MailBox_read_Date <= '1';
            end if;
            if MailBox_read_Date = '1' and wb_ack_i_MailBox = '1' then
                MailBox_read_Date <= '0';
                MailBox_read_Data <= '1';
            end if;
            if MailBox_read_Data = '1' and wb_ack_i_MailBox = '1' then
                MailBox_read_Data <= '0';
                MailBox_write_UART_Frame <= '1';
            end if;
            if MailBox_write_UART_Frame = '1' then
                MailBox_write_UART_Frame <= '0';
                MailBox_read <= '0';
            end if;
            
            if MailBox_write_UART_Frame = '1' then
                UART_write <= '1';
            end if;
            if UART_write = '1' and UART_Tx_Byte_Counter = 9 and wb_ack_i_UART = '1' then
                UART_write <= '0';
            end if;
            
        end if;
    end process;

end Sequencer_behavior;