----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART_Sequencer
--
-- Description:     UART Sequencer
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
use ieee.math_real.log2;
use ieee.math_real.ceil;

library UART_LIB;
use UART_LIB.UART_PACK.all;

entity UART_Sequencer is
    generic
    (
        GlobalClockFrequency    : integer := 50e6;  -- design frequency
        Ser_nDes                : std_logic := '1';   -- 1 => Serializer ; 0 => Deserializer
        BaudRate                : integer := 9600;
        DataBits                : integer := 8;
        ParityBit               : integer := 0;
        StopBits                : integer := 1
    );
    port
    (
    -- Global inputs :
        rst         : in std_logic;        -- Global asynchronous reset
        clk         : in std_logic;        -- Global clock
    
    -- UART interface :
        Start       : in std_logic;
        TopBit      : out std_logic;
        Busy        : out std_logic
    );
end UART_Sequencer;

architecture behavior of UART_Sequencer is

    constant SerialPeriod   : integer := integer(real(GlobalClockFrequency)/real(BaudRate)) - 1;
    impure function FrameLengthCalc return integer is
        variable Tmp : integer;
    begin
        if Ser_nDes = '0' then
            Tmp := DataBits + ParityBit + StopBits;
        else
            Tmp := DataBits + ParityBit + StopBits + 1;
        end if;
        return Tmp;
    end function;
    constant FrameLength    : integer := FrameLengthCalc;
    
    signal ClkCounter       : unsigned(integer(ceil(log2(real(SerialPeriod)))) - 1 downto 0);
    signal BitCounter       : unsigned(integer(ceil(log2(real(FrameLength)))) - 1 downto 0);
    signal Idle             : std_logic;
	signal TopBit_tmp		: std_logic;

begin
    ClkCounter_process : process (rst, clk)
    begin
        if rst = '1' then
            ClkCounter <= (others => '0');
        elsif rising_edge(clk) then
            
            if Start = '1' then
                if Ser_nDes = '0' then
                    ClkCounter <= to_unsigned(SerialPeriod/2, integer(ceil(log2(real(SerialPeriod)))));
                else
                    ClkCounter <= to_unsigned(SerialPeriod, integer(ceil(log2(real(SerialPeriod)))));
                end if;
            end if;
            
            
            if Idle = '0' then
                if ClkCounter < SerialPeriod then
                    ClkCounter <= ClkCounter + 1;
                else
                    ClkCounter <= (others => '0');
                end if;
            end if;
			
        end if;
    end process;
    
    BitCounter_process : process(rst, clk)
    begin
        if rst = '1' then
            BitCounter <= (others => '0');
        elsif rising_edge(clk) then
        
            if Start = '1' then
                BitCounter <= (others => '0');
            end if;
            
            if ClkCounter = SerialPeriod then
                if BitCounter < FrameLength then
                    BitCounter <= BitCounter + 1;
                else
                    BitCounter <= (others => '0');
                end if;
            end if;
            
        end if;
    end process;
    
    TopBit_process : process(rst, clk)
    begin
        if rst = '1' then
            TopBit_tmp <= '0';
        elsif rising_edge(clk) then
            TopBit_tmp <= '0';
            
            if ClkCounter = SerialPeriod then
                TopBit_tmp <= '1';
            end if;
            
        end if;
    end process;
	TopBit <= TopBit_tmp;
    
    IdleManagement_process : process(rst, clk)
    begin
        if rst = '1' then
            Idle <= '1';
        elsif rising_edge(clk) then
            
            if Start = '1' then
                Idle <= '0';
            end if;
            
			if Idle = '0' and TopBit_tmp = '1' and ClkCounter = 0 and BitCounter = 0 then
				Idle <= '1';
			end if;
            
        end if;
    end process;
    Busy <= (not Idle) or Start;

end behavior;