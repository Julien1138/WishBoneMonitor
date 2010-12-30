----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     WishBoneMonitor_Pack
--
-- Description:     WishBoneMonitor Pack
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

library WishBoneMonitor_Lib;

package WishBoneMonitor_Pack is
    
    component WishBoneMonitor
        generic
        (
            WB_Addr_Width   : integer
        );
        port
        (
        -- Global inputs :
            rst                 : in std_logic;         -- global asynchronous reset
            clk                 : in std_logic;         -- global clock
            
        -- UART interface :
            Rx                  : in std_logic;         -- Rx serial signal
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
    end component;
    
    component WBM_Sequencer
        generic
        (
            WB_Addr_Width   : integer
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
    end component;
    
end package;