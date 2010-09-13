----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART_Deserializer
--
-- Description:     UART deserializer
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

entity UART_Deserializer is
    generic
    (
        DataBits        : integer := 8;
        Endianness      : std_logic := '0'; -- 0 => LSB first; 1 => MSB first
        ParityBit       : integer := 0;
        Parity          : std_logic := '0'; -- 0 => even Parity; 1 => odd Parity
        StopBits        : integer := 1
    );
    port
    (
    -- Global inputs :
        rst             : in std_logic;        -- global asynchronous reset
        clk             : in std_logic;        -- global clock
        
    -- Sequencer interface :
        TopBit          : in std_logic;
        Busy            : in std_logic;
        
    -- Serial interface :
        Rx              : in std_logic;    -- Tx serial signal
        
    -- DATA interface :
        ParityError     : out std_logic;
        DataOut         : out std_logic_vector(DataBits - 1 downto 0)
    );
end UART_Deserializer;

architecture Behavior of UART_Deserializer is

    signal OutBuff      : std_logic_vector(DataBits + ParityBit + StopBits downto 1);
    
    impure function Parity_Gen return std_logic is
        variable Parity_var : std_logic;
    begin
        Parity_var := Parity;
        for i in 1 to DataBits loop
            Parity_var := Parity_var xor OutBuff(i);
        end loop;
        return Parity_var;
    end function;
    
begin

    process(rst, clk)
    begin
        if rst = '1' then
            ParityError <= '0';
            DataOut <= (others => '0');
            OutBuff <= (others => '0');
        elsif rising_edge(clk) then
            
            if Busy = '0' then
                -- Data
                for i in 1 to DataBits loop
                    if Endianness = '0' then
                        DataOut(i-1) <= OutBuff(i);
                    else
                        DataOut(DataBits - i) <= OutBuff(i);
                    end if;
                end loop;
                -- Parity Check
                if ParityBit = 1 then
                    ParityError <= Parity_Gen xor OutBuff(DataBits + ParityBit);
                end if;
            end if;
            
            if TopBit = '1' then
                OutBuff <= Rx & OutBuff(DataBits + ParityBit + StopBits downto 2);
                ParityError <= '0';
            end if;
        end if;
    end process;

end Behavior;