----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     WishBoneMonitor_Exemple
--
-- Description:     WishBoneMonitor Top Level Example
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

entity WishBoneMonitor_Exemple is
    generic
    (
        WB_Addr_Width   : integer := 4
    );
    port
    (
    -- Global inputs :
        rst                 : in std_logic;         -- global asynchronous reset
        clk                 : in std_logic;         -- global clock
        
    -- Serial interface :
        Rx                  : in std_logic;         -- Rx serial signal
        Tx                  : out std_logic;        -- Tx serial signal
      
    -- Led Interface
        Led                 : out std_logic_vector(7 downto 0);
    
    -- Slider Interface
        Slider              : in std_logic_vector(7 downto 0)
    );
end WishBoneMonitor_Exemple;

architecture WishBoneMonitor_Exemple_behavior of WishBoneMonitor_Exemple is
    
    signal rst_sync     : std_logic;
    signal rst_sync_tmp : std_logic;

    component InputFilter
        generic
        (
            Length  : integer := 2
        );
        port
        (
        -- Global inputs :
            rst     : in std_logic;         -- global asynchronous reset
            clk     : in std_logic;         -- global clock
            
        -- Reset Commands :
            Input   : in std_logic;
            Output  : out std_logic
        );
    end component;
    signal Rx_sync  : std_logic;
    
--
--! WishBoneMonitor
--
    -- Internal WishBone signals between UART and Sequencer
    signal wb_we_SequencerToUART    : std_logic;
    signal wb_dat_SequencerToUART   : std_logic_vector(7 downto 0);
    signal wb_dat_UARTToSequencer   : std_logic_vector(7 downto 0);
    signal wb_cyc_SequencerToUART   : std_logic;
    signal wb_stb_SequencerToUART   : std_logic;
    signal wb_ack_UARTToSequencer   : std_logic;
    signal wb_rty_UARTToSequencer   : std_logic;
    signal wb_wait_UARTToSequencer  : std_logic;

    -- Internal WishBone signals between UART and Sequencer
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
    
    -- Internal WishBone signals between MailBox and Slave
    signal wb_we_MailBoxToUUT   : std_logic;
    signal wb_adr_MailBoxToUUT  : std_logic_vector(WB_Addr_Width - 1 downto 0);
    signal wb_dat_MailBoxToUUT  : std_logic_vector(31 downto 0);
    signal wb_dat_UUTToMailBox  : std_logic_vector(31 downto 0);
    signal wb_cyc_MailBoxToUUT  : std_logic;
    signal wb_stb_MailBoxToUUT  : std_logic;
    signal wb_ack_UUTToMailBox  : std_logic;
    
    signal Led_Reg      : std_logic_vector(7 downto 0);
    
begin
    
    Rst_Sync_process : process(rst, clk)
    begin
        if rst = '1' then
            rst_sync <= '1';
            rst_sync_tmp <= '1';
        elsif rising_edge(clk) then
            rst_sync_tmp <= rst;
            rst_sync <= rst_sync_tmp;
        end if;
    end process;

    Rx_Filter : InputFilter
    generic map
    (
        Length => 10
    )
    port map
    (
        rst => rst_sync,
        clk => clk,
        Input => Rx,
        Output => Rx_sync
    );
    
    WishBoneMonitor_Inst : WishBoneMonitor
    generic map
    (
        WB_Addr_Width => 4
    )
    port map
    (
    -- Global inputs :
        rst             => rst_sync,
        clk             => clk,
        
    -- UART interface :
        Rx              => Rx_sync,
        Tx              => Tx,
        
    -- UUT interface :
        wb_we_o_UUT     => wb_we_MailBoxToUUT,
        wb_adr_o_UUT    => wb_adr_MailBoxToUUT,
        wb_dat_o_UUT    => wb_dat_MailBoxToUUT,
        wb_dat_i_UUT    => wb_dat_UUTToMailBox,
        wb_cyc_o_UUT    => wb_cyc_MailBoxToUUT,
        wb_stb_o_UUT    => wb_stb_MailBoxToUUT,
        wb_ack_i_UUT    => wb_ack_UUTToMailBox
    );
    
    Ack_process : process(rst_sync, clk)
    begin
        if rst_sync = '1' then
            wb_ack_UUTToMailBox <= '0';
        elsif rising_edge(clk) then
            wb_ack_UUTToMailBox <= '0';
            
            if wb_cyc_MailBoxToUUT = '1' and wb_stb_MailBoxToUUT = '1' and wb_ack_UUTToMailBox = '0' then
                wb_ack_UUTToMailBox <= '1';
            end if;
            
        end if;
    end process;
    
    Register_process : process(rst_sync, clk)
    begin
        if rst_sync = '1' then
            Led_Reg <= (others => '0');
        elsif rising_edge(clk) then
            if wb_cyc_MailBoxToUUT = '1' and wb_stb_MailBoxToUUT = '1' then
                if wb_adr_MailBoxToUUT(0) = '0' then
                    if wb_we_MailBoxToUUT = '1' then
                        Led_Reg <= wb_dat_MailBoxToUUT(7 downto 0);
                    else
                        wb_dat_UUTToMailBox <= X"000000" & Led_Reg;
                    end if;
                else
                    if wb_we_MailBoxToUUT = '0' then
                        wb_dat_UUTToMailBox <= X"000000" & Slider;
                    end if;
                end if;
            end if;
        end if;
    end process;
    Led <= Led_Reg;

end WishBoneMonitor_Exemple_behavior;