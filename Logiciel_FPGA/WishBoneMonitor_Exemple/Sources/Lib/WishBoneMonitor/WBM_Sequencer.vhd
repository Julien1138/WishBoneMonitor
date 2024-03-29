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

entity WBM_Sequencer is
    generic
    (
        WB_Addr_Width   : integer := 4
    );
    port
    (
        wb_clk_i            : in std_logic;
        wb_rst_i            : in std_logic;
        
    -- Host interface
        wb_we_o_Host        : out std_logic;
        wb_dat_o_Host       : out std_logic_vector(7 downto 0);
        wb_dat_i_Host       : in std_logic_vector(7 downto 0);
        wb_cyc_o_Host       : out std_logic;
        wb_stb_o_Host       : out std_logic;
        wb_ack_i_Host       : in std_logic;
        wb_rty_i_Host       : in std_logic;
        wb_wait_i_Host      : in std_logic;
        
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
        
        RTC_time            : in std_logic_vector(15 downto 0);
        Reset_MailBox       : out std_logic
    );
end WBM_Sequencer;

architecture WBM_Sequencer_behavior of WBM_Sequencer is

    signal Host_read                : std_logic;
    signal Host_Rx_Frame            : std_logic_vector(79 downto 0);
    signal Host_Rx_Frame_Header     : std_logic_vector(7 downto 0);
    signal Host_Rx_Frame_Address    : std_logic_vector(15 downto 0);
    signal Host_Rx_Frame_Data       : std_logic_vector(31 downto 0);
    signal Host_Rx_Frame_Period     : std_logic_vector(15 downto 0);
    signal Host_Rx_Frame_CheckSum   : std_logic_vector(7 downto 0);
    signal Host_Rx_Frame_Valid      : std_logic;
    signal Host_Rx_Byte_Counter     : std_logic_vector(3 downto 0);
    signal WaitForMailBoxReset      : std_logic;
    signal RxTimeout_Timer          : std_logic_vector(13 downto 0);
    
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
    signal MailBox_write_Host_Frame : std_logic;
    signal MailBox_Direction_read   : std_logic;
    signal MailBox_Address_read     : std_logic_vector(WB_Addr_Width - 1 downto 0);
    signal Host_Tx_Frame_Address    : std_logic_vector(15 downto 0);
    signal MailBox_Data_read        : std_logic_vector(31 downto 0);
    signal MailBox_Date_read        : std_logic_vector(15 downto 0);
    signal Host_Tx_Frame_CheckSum   : std_logic_vector(7 downto 0);
    
    signal Host_write           : std_logic;
    signal Host_Tx_Frame        : std_logic_vector(79 downto 0);
    signal Host_Tx_Byte_Counter : std_logic_vector(3 downto 0);

begin
    
--===========================================================================
--         Gestion des ports WishBone
--===========================================================================
    
    -- Interface Host
    Host_Interface_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            wb_we_o_Host <= '0';
            wb_cyc_o_Host <= '0';
            wb_stb_o_Host <= '0';
            wb_dat_o_Host <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if Host_read = '1' then
                if wb_ack_i_Host = '0' then
                    wb_we_o_Host <= '0';
                    wb_cyc_o_Host <= '1';
                    wb_stb_o_Host <= '1';
                else
                    wb_we_o_Host <= '0';
                    wb_cyc_o_Host <= '0';
                    wb_stb_o_Host <= '0';
                end if;
            elsif Host_write = '1' then
                if wb_ack_i_Host = '0' and wb_rty_i_Host = '0' then
                    wb_we_o_Host <= '1';
                    wb_cyc_o_Host <= '1';
                    wb_stb_o_Host <= '1';
                    wb_dat_o_Host <= Host_Tx_Frame(79 downto 72);
                else
                    wb_we_o_Host <= '0';
                    wb_cyc_o_Host <= '0';
                    wb_stb_o_Host <= '0';
                end if;
            else
                wb_we_o_Host <= '0';
                wb_cyc_o_Host <= '0';
                wb_stb_o_Host <= '0';
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
    
    Read_Host_Frame_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            Host_Rx_Frame <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if Host_read = '1' and wb_ack_i_Host = '1' then
                Host_Rx_Frame <= Host_Rx_Frame(71 downto 0) & wb_dat_i_Host;
            end if;
        end if;
    end process;
    Host_Rx_Frame_Header    <= Host_Rx_Frame(79 downto 72);
    Host_Rx_Frame_Address   <= Host_Rx_Frame(71 downto 56);
    Host_Rx_Frame_Data      <= Host_Rx_Frame(55 downto 24);
    Host_Rx_Frame_Period    <= Host_Rx_Frame(23 downto 8);
    Host_Rx_Frame_CheckSum  <= Host_Rx_Frame( 7 downto  0) xor
                               Host_Rx_Frame(15 downto  8) xor
                               Host_Rx_Frame(23 downto 16) xor
                               Host_Rx_Frame(31 downto 24) xor
                               Host_Rx_Frame(39 downto 32) xor
                               Host_Rx_Frame(47 downto 40) xor
                               Host_Rx_Frame(55 downto 48) xor
                               Host_Rx_Frame(63 downto 56) xor
                               Host_Rx_Frame(71 downto 64) xor
                               Host_Rx_Frame(79 downto 72);
    Host_Rx_Frame_Valid <= '1' when Host_Rx_Frame_CheckSum = 0 and Host_Rx_Byte_Counter = 0 else
                           '0';
    
    Host_Rx_Byte_Counter_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            Host_Rx_Byte_Counter <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if Host_read = '1' and wb_ack_i_Host = '1' then
                if Host_Rx_Byte_Counter = 0 then
                    Host_Rx_Byte_Counter <= X"9";
                elsif RxTimeout_Timer = 0 then
                    Host_Rx_Byte_Counter <= X"9";
                else
                    Host_Rx_Byte_Counter <= Host_Rx_Byte_Counter - 1;
                end if;
            end if;
        end if;
    end process;
    
    MailBox_write_Latch_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            MailBox_Direction_to_write <= '0';
            MailBox_Address_to_write <= (others => '0');
            MailBox_Data_to_write <= (others => '0');
            MailBox_Period_to_write <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_write_Latch = '1' then
                MailBox_Address_to_write <= Host_Rx_Frame_Address(WB_Addr_Width - 1 downto 0);
                MailBox_Direction_to_write <= Host_Rx_Frame_Header(0);
                MailBox_Data_to_write <= Host_Rx_Frame_Data;
                MailBox_Period_to_write <= Host_Rx_Frame_Period;
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
    Host_Tx_Frame_CheckSum <= "0000000" & MailBox_Direction_read xor
                              Host_Tx_Frame_Address(15 downto  8) xor
                              Host_Tx_Frame_Address( 7 downto  0) xor
                              MailBox_Data_read(31 downto 24) xor
                              MailBox_Data_read(23 downto 16) xor
                              MailBox_Data_read(15 downto  8) xor
                              MailBox_Data_read( 7 downto  0) xor
                              MailBox_Date_read(15 downto  8) xor
                              MailBox_Date_read( 7 downto  0);
    
    Host_Tx_Frame_Address_generate : for i in 0 to 15 generate
        Host_Tx_Frame_Address_if1 : if i < WB_Addr_Width generate
            Host_Tx_Frame_Address(i) <= MailBox_Address_read(i);
        end generate;
        Host_Tx_Frame_Address_if2 : if i >= WB_Addr_Width generate
            Host_Tx_Frame_Address(i) <= '0';
        end generate;
    end generate;
    
    Host_Tx_Byte_Counter_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            Host_Tx_Byte_Counter <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if Host_write = '1' and wb_ack_i_Host = '1' then
                if Host_Tx_Byte_Counter = 9 then
                    Host_Tx_Byte_Counter <= (others => '0');
                else
                    Host_Tx_Byte_Counter <= Host_Tx_Byte_Counter + 1;
                end if;
            end if;
        end if;
    end process;
    
    Write_Host_Frame_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            Host_Tx_Frame <= (others => '0');
        elsif rising_edge(wb_clk_i) then
            if MailBox_write_Host_Frame = '1' then
                Host_Tx_Frame <= "0000000" & MailBox_Direction_read &
                                 Host_Tx_Frame_Address &
                                 MailBox_Data_read &
                                 MailBox_Date_read &
                                 Host_Tx_Frame_CheckSum;
            end if;
            if Host_write = '1' and wb_ack_i_Host = '1' then
                Host_Tx_Frame <= Host_Tx_Frame(71 downto 0) & Host_Tx_Frame(79 downto 72);
            end if;
        end if;
    end process;
    
    Reset_MailBox_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            WaitForMailBoxReset <= '1';
            Reset_MailBox <= '1';
        elsif rising_edge(wb_clk_i) then
            Reset_MailBox <= '0';
            
            if Host_Rx_Frame_Valid = '1' and Host_Rx_Frame_Header = X"55" and WaitForMailBoxReset = '1' then
                Reset_MailBox <= '1';
                WaitForMailBoxReset <= '0';
            end if;
            
            if Host_read = '1' and wb_ack_i_Host = '1' then
                WaitForMailBoxReset <= '1';
            end if;
            
        end if;
    end process;
    
    RxTimeout_Timer_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            RxTimeout_Timer <= (others => '1');
        elsif rising_edge(wb_clk_i) then
            
            if Host_read = '1' and wb_ack_i_Host = '1' then
                RxTimeout_Timer <= (others => '1');
            elsif RxTimeout_Timer /= 0 then
                RxTimeout_Timer <= RxTimeout_Timer - 1;
            end if;
            
        end if;
    end process;

    Operation_sequencer_process : process(wb_rst_i, wb_clk_i)
    begin
        if wb_rst_i = '1' then
            Host_read <= '0';
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
            MailBox_write_Host_Frame <= '0';
            Host_write <= '0';
        elsif rising_edge(wb_clk_i) then
        
            if Host_read = '0' and wb_wait_i_Host = '1' then
                Host_read <= '1';
            end if;
            if Host_read = '1' and wb_ack_i_Host = '1' then
                Host_read <= '0';
            end if;
            
            if MailBox_write = '0' and MailBox_written = '0' and Host_Rx_Frame_Valid = '1' and Host_Rx_Frame_Header(7 downto 1) = "0000000" then
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
            if Host_Rx_Frame_Valid = '0' then
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
                MailBox_write_Host_Frame <= '1';
            end if;
            if MailBox_write_Host_Frame = '1' then
                MailBox_write_Host_Frame <= '0';
                MailBox_read <= '0';
            end if;
            
            if MailBox_write_Host_Frame = '1' then
                Host_write <= '1';
            end if;
            if Host_write = '1' and Host_Tx_Byte_Counter = 9 and wb_ack_i_Host = '1' then
                Host_write <= '0';
            end if;
            
        end if;
    end process;

end WBM_Sequencer_behavior;