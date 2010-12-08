----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART_Serializer
--
-- Description:     UART Serializer
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

entity UART_Serializer is
    generic
    (
        DataBits        : integer := 8;
        Endianness      : std_logic := '0'; -- 0 => LSB first; 1 => MSB first
        ParityBit       : integer := 0;
        Parity          : std_logic := '0'; -- 0 => even Parity; 1 => odd Parity
        StopBits        : integer := 2
    );
    port
    (
    -- Global inputs :
        rst             : in std_logic;        -- global asynchronous reset
        clk             : in std_logic;        -- global clock
        
    -- Sequencer interface :
        TopBit          : in std_logic;
        
    -- Serial interface :
        Tx              : out std_logic;    -- Tx serial signal
        
    -- DATA interface :
        LoadData        : in std_logic;
        DataIn          : in std_logic_vector(DataBits - 1 downto 0)
    );
end UART_Serializer;

architecture Behavior of UART_Serializer is

    impure function Parity_Gen return std_logic is
        variable Parity_var : std_logic;
    begin
        Parity_var := Parity;
        for i in 0 to DataBits-1 loop
            Parity_var := Parity_var xor DataIn(i);
        end loop;
        return Parity_var;
    end function;
    
    signal InBuff       : std_logic_vector(StopBits + DataBits + ParityBit + 1 downto 0);
        
begin

    process(rst, clk)
    begin
        if rst = '1' then
            Tx <= '1';
            InBuff <= (others => '0');
        elsif rising_edge(clk) then
            
            if LoadData = '1' then
                -- StartBit
                InBuff(0) <= '0';
                -- Data
                for i in 1 to DataBits loop
                    if Endianness = '0' then
                        InBuff(i) <= DataIn(i-1);
                    else
                        InBuff(i) <= DataIn(DataBits - i);
                    end if;
                end loop;
                -- Bit de Parité
                if ParityBit = 1 then
                    InBuff(DataBits + ParityBit) <= Parity_Gen;
                end if;
                -- StopBits
                InBuff(DataBits + ParityBit + 1) <= '1';
                InBuff(DataBits + ParityBit + StopBits) <= '1';
                -- Repos du bus
                InBuff(DataBits + ParityBit + StopBits + 1) <= '1';
            end if;
            
            if TopBit = '1' then
                Tx <= InBuff(0);
                InBuff <= InBuff(0) & InBuff(DataBits + ParityBit + StopBits + 1 downto 1);
            end if;
        end if;
    end process;

end Behavior;