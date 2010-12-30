----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     WishBoneMonitor
--
-- Description:     WishBoneMonitor Top Level
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

library UART_LIB;
use UART_LIB.UART_PACK.all;

library MailBox_Lib;
use MailBox_Lib.MailBox_Pack.all;

library WishBoneMonitor_Lib;
use WishBoneMonitor_Lib.WishBoneMonitor_Pack.all;

entity WishBoneMonitor is
    generic
    (
        WB_Addr_Width   : integer := 4
    );
    port
    (
    -- Global inputs :
        rst                 : in std_logic;         -- global asynchronous reset
        clk                 : in std_logic;         -- global clock
        
    -- UART interface :
        Rx                  : in std_logic := '1';  -- Rx serial signal
        Tx                  : out std_logic;        -- Tx serial signal
        
    -- UUT interface :
        wb_we_o_UUT         : out std_logic;
        wb_adr_o_UUT        : out std_logic_vector(WB_Addr_Width - 1 downto 0);
        wb_dat_o_UUT        : out std_logic_vector(31 downto 0);
        wb_dat_i_UUT        : in std_logic_vector(31 downto 0);
        wb_cyc_o_UUT        : out std_logic;
        wb_stb_o_UUT        : out std_logic;
        wb_ack_i_UUT        : in std_logic
    );
end WishBoneMonitor;

architecture WishBoneMonitor_behavior of WishBoneMonitor is

--
--! WBM_Sequencer
--
    -- Internal WishBone signals between Host and Sequencer
    signal wb_we_SequencerToHost    : std_logic;
    signal wb_dat_SequencerToHost   : std_logic_vector(7 downto 0);
    signal wb_dat_HostToSequencer   : std_logic_vector(7 downto 0);
    signal wb_cyc_SequencerToHost   : std_logic;
    signal wb_stb_SequencerToHost   : std_logic;
    signal wb_ack_HostToSequencer   : std_logic;
    signal wb_rty_HostToSequencer   : std_logic;
    signal wb_wait_HostToSequencer  : std_logic;

    -- Internal WishBone signals between Host and Sequencer
    signal wb_we_SequencerToMailBox     : std_logic;
    signal wb_adr_SequencerToMailBox    : std_logic_vector(WB_Addr_Width + 2 downto 0);
    signal wb_dat_SequencerToMailBox    : std_logic_vector(31 downto 0);
    signal wb_dat_MailBoxToSequencer    : std_logic_vector(31 downto 0);
    signal wb_cyc_SequencerToMailBox    : std_logic;
    signal wb_stb_SequencerToMailBox    : std_logic;
    signal wb_ack_MailBoxToSequencer    : std_logic;
    signal wb_dtr_MailBoxToSequencer    : std_logic;
    signal wb_atr_MailBoxToSequencer    : std_logic_vector(WB_Addr_Width downto 0);
    signal RTC_time                     : std_logic_vector(15 downto 0);
    signal Reset_MailBox                : std_logic;
    
begin
    
    Host_inst : UART_wb
    generic map
    (
        GlobalClockFrequency => 50e6,
        BaudRate => 115200,
        DataBits => 8,
        Endianness => '0',
        ParityBit => 0,
        Parity => '0',
        StopBits => 1,
        BufferSize => 256,
        WB_Data_Width => 8
    )
    port map
    (
        wb_clk_i => clk,
        wb_rst_i => rst,
        
        wb_we_i => wb_we_SequencerToHost,
        wb_dat_i => wb_dat_SequencerToHost,
        wb_dat_o => wb_dat_HostToSequencer,
        wb_cyc_i => wb_cyc_SequencerToHost,
        wb_stb_i => wb_stb_SequencerToHost,
        wb_ack_o => wb_ack_HostToSequencer,
        wb_rty_o => wb_rty_HostToSequencer,
        wb_wait_o => wb_wait_HostToSequencer,
        
        Rx => Rx,
        Tx => Tx
    );
    
    Sequencer_inst : WBM_Sequencer
    generic map
    (
        WB_Addr_Width => 4
    )
    port map
    (
        wb_clk_i => clk,
        wb_rst_i => rst,
        
        wb_we_o_Host => wb_we_SequencerToHost,
        wb_dat_o_Host => wb_dat_SequencerToHost,
        wb_dat_i_Host => wb_dat_HostToSequencer,
        wb_cyc_o_Host => wb_cyc_SequencerToHost,
        wb_stb_o_Host => wb_stb_SequencerToHost,
        wb_ack_i_Host => wb_ack_HostToSequencer,
        wb_rty_i_Host => wb_rty_HostToSequencer,
        wb_wait_i_Host => wb_wait_HostToSequencer,
        
        wb_we_o_MailBox => wb_we_SequencerToMailBox,
        wb_adr_o_MailBox => wb_adr_SequencerToMailBox,
        wb_dat_o_MailBox => wb_dat_SequencerToMailBox,
        wb_dat_i_MailBox => wb_dat_MailBoxToSequencer,
        wb_cyc_o_MailBox => wb_cyc_SequencerToMailBox,
        wb_stb_o_MailBox => wb_stb_SequencerToMailBox,
        wb_ack_i_MailBox => wb_ack_MailBoxToSequencer,
        wb_dtr_i_MailBox => wb_dtr_MailBoxToSequencer,
        wb_atr_i_MailBox => wb_atr_MailBoxToSequencer,
        
        RTC_time => RTC_time,
        Reset_MailBox => Reset_MailBox
    );
    
    MailBox_inst : MailBox
    generic map
    (
        GlobalClockFrequency => 50e6,
        RTCClockFrequency => 1e3,
        WB_Addr_Width => 4,
        WB_Data_Width => 32,
        RTC_time_Width => 16
    )
    port map
    (
        wb_clk_i => clk,
        wb_rst_i => Reset_MailBox,
        
        wb_we_i_Slave => wb_we_SequencerToMailBox,
        wb_adr_i_Slave => wb_adr_SequencerToMailBox,
        wb_dat_i_Slave => wb_dat_SequencerToMailBox,
        wb_dat_o_Slave => wb_dat_MailBoxToSequencer,
        wb_cyc_i_Slave => wb_cyc_SequencerToMailBox,
        wb_stb_i_Slave => wb_stb_SequencerToMailBox,
        wb_ack_o_Slave => wb_ack_MailBoxToSequencer,
        wb_dtr_o_Slave => wb_dtr_MailBoxToSequencer,
        wb_atr_o_Slave => wb_atr_MailBoxToSequencer,
        
        wb_we_o_Master => wb_we_o_UUT,
        wb_adr_o_Master => wb_adr_o_UUT,
        wb_dat_o_Master => wb_dat_o_UUT,
        wb_dat_i_Master => wb_dat_i_UUT,
        wb_cyc_o_Master => wb_cyc_o_UUT,
        wb_stb_o_Master => wb_stb_o_UUT,
        wb_ack_i_Master => wb_ack_i_UUT,
        
        RTC_time => RTC_time
    );

end WishBoneMonitor_behavior;