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
        
    -- Serial interface :
        Rx                  : in std_logic;         -- Rx serial signal
        Tx                  : out std_logic;        -- Tx serial signal
      
    -- Led Interface
        Led                 : out std_logic_vector(7 downto 0);
    
    -- Slider Interface
        Slider              : in std_logic_vector(7 downto 0)
    );
end WishBoneMonitor;

architecture WishBoneMonitor_behavior of WishBoneMonitor is

    component Input_Synchro
        generic
        (
            SampleLength        : integer := 2
        );
        port
        (
        -- Global inputs :
            rst                 : in std_logic;         -- global asynchronous reset
            clk                 : in std_logic;         -- global clock
            
        -- Reset Commands :
            UnSynchronized      : in std_logic;
            Synchronized        : out std_logic
        );
    end component;
    signal Rx_sync  : std_logic;

    component Sequencer
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
    end component;

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

    Rx_Synchro : Input_Synchro
    generic map
    (
        SampleLength => 4
    )
    port map
    (
        rst => rst,
        clk => clk,
        UnSynchronized => Rx,
        Synchronized => Rx_sync
    );
    
    UART_inst : UART_wb
    generic map
    (
        GlobalClockFrequency => 50e6,
        BaudRate => 115200,
        DataBits => 8,
        Endianness => '0',
        ParityBit => 0,
        Parity => '0',
        StopBits => 2,
        BufferSize => 64,
        WB_Data_Width => 8
    )
    port map
    (
        wb_clk_i => clk,
        wb_rst_i => rst,
        
        wb_we_i => wb_we_SequencerToUART,
        wb_dat_i => wb_dat_SequencerToUART,
        wb_dat_o => wb_dat_UARTToSequencer,
        wb_cyc_i => wb_cyc_SequencerToUART,
        wb_stb_i => wb_stb_SequencerToUART,
        wb_ack_o => wb_ack_UARTToSequencer,
        wb_rty_o => wb_rty_UARTToSequencer,
        wb_wait_o => wb_wait_UARTToSequencer,
        
        Rx => Rx_sync,
        Tx => Tx
    );
    
    Sequencer_inst : Sequencer
    generic map
    (
        WB_Addr_Width => 4
    )
    port map
    (
        wb_clk_i => clk,
        wb_rst_i => rst,
        
        wb_we_o_UART => wb_we_SequencerToUART,
        wb_dat_o_UART => wb_dat_SequencerToUART,
        wb_dat_i_UART => wb_dat_UARTToSequencer,
        wb_cyc_o_UART => wb_cyc_SequencerToUART,
        wb_stb_o_UART => wb_stb_SequencerToUART,
        wb_ack_i_UART => wb_ack_UARTToSequencer,
        wb_rty_i_UART => wb_rty_UARTToSequencer,
        wb_wait_i_UART => wb_wait_UARTToSequencer,
        
        wb_we_o_MailBox => wb_we_SequencerToMailBox,
        wb_adr_o_MailBox => wb_adr_SequencerToMailBox,
        wb_dat_o_MailBox => wb_dat_SequencerToMailBox,
        wb_dat_i_MailBox => wb_dat_MailBoxToSequencer,
        wb_cyc_o_MailBox => wb_cyc_SequencerToMailBox,
        wb_stb_o_MailBox => wb_stb_SequencerToMailBox,
        wb_ack_i_MailBox => wb_ack_MailBoxToSequencer,
        wb_dtr_i_MailBox => wb_dtr_MailBoxToSequencer,
        wb_atr_i_MailBox => wb_atr_MailBoxToSequencer,
        
        RTC_time => RTC_time
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
        wb_rst_i => rst,
        wb_we_i_Slave => wb_we_SequencerToMailBox,
        wb_adr_i_Slave => wb_adr_SequencerToMailBox,
        wb_dat_i_Slave => wb_dat_SequencerToMailBox,
        wb_dat_o_Slave => wb_dat_MailBoxToSequencer,
        wb_cyc_i_Slave => wb_cyc_SequencerToMailBox,
        wb_stb_i_Slave => wb_stb_SequencerToMailBox,
        wb_ack_o_Slave => wb_ack_MailBoxToSequencer,
        wb_dtr_o_Slave => wb_dtr_MailBoxToSequencer,
        wb_atr_o_Slave => wb_atr_MailBoxToSequencer,
        
        wb_we_o_Master => wb_we_MailBoxToUUT,
        wb_adr_o_Master => wb_adr_MailBoxToUUT,
        wb_dat_o_Master => wb_dat_MailBoxToUUT,
        wb_dat_i_Master => wb_dat_UUTToMailBox,
        wb_cyc_o_Master => wb_cyc_MailBoxToUUT,
        wb_stb_o_Master => wb_stb_MailBoxToUUT,
        wb_ack_i_Master => wb_ack_UUTToMailBox,
        
        RTC_time => RTC_time
    );
    
    Ack_process : process(rst, clk)
    begin
        if rst = '1' then
            wb_ack_UUTToMailBox <= '0';
        elsif rising_edge(clk) then
            wb_ack_UUTToMailBox <= '0';
            
            if wb_cyc_MailBoxToUUT = '1' and wb_stb_MailBoxToUUT = '1' and wb_ack_UUTToMailBox = '0' then
                wb_ack_UUTToMailBox <= '1';
            end if;
            
        end if;
    end process;
    
    Register_process : process(rst, clk)
    begin
        if rst = '1' then
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

end WishBoneMonitor_behavior;