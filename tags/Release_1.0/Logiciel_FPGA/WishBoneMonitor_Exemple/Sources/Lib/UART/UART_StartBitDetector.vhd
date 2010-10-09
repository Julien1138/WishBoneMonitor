----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART_StartBitDetector
--
-- Description:     UART Start bit detector (on Rx)
--
-- --------------------------------------------------------------------------------
-- Revision List
-- Version      Author(s)       Date        Changes
--
-- 1.0          J.Aupart        13/11/09    Creation
-----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library UART_LIB;
use UART_LIB.UART_PACK.all;

entity UART_StartBitDetector is
    port
    (
    -- Global inputs :
        rst                 : in std_logic;        -- global asynchronous reset
        clk                 : in std_logic;        -- global clock
        
    -- Serial interface :
        Rx                  : in std_logic;        -- Rx serial signal
        
    -- Sequencer interface :
        WaitForStartBit     : in std_logic;
        StartBitDetected    : out std_logic
    );
end UART_StartBitDetector;

architecture behavior of UART_StartBitDetector is

    signal Rx_prev  : std_logic;

begin
    process (rst, clk, Rx)
    begin
        if rst = '1' then
            StartBitDetected <= '0';
            Rx_prev <= Rx;
        elsif rising_edge(clk) then
            StartBitDetected <= '0';
            
            -- Conditions for the start bit detection
            if WaitForStartBit = '1' then
                if Rx = '0' and Rx_prev = '1' then    -- Rx falling edge
                    StartBitDetected <= '1';
                end if;
            end if;

            Rx_prev <= Rx;
        end if;
    end process;
end behavior;