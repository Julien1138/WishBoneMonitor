----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     Input_Synchro
--
-- Description:     Synchronisation d'un signal externe
--                  Le retard induit par ce bloc sur le signal entrant
--                  est égale à SampleLength + 1 coup d'horloge clk
-- 
-- --------------------------------------------------------------------------------
-- Revision List
-- Version      Author(s)       Date        Changes
--
-- 1.0          J.Aupart        11/12/09    Creation
-----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Input_Synchro is
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
end Input_Synchro;

architecture behavior of Input_Synchro is

    signal SynchroVect_cur  : std_logic_vector(SampleLength-1 downto 0);
    signal SynchroVect_nxt  : std_logic_vector(SampleLength-1 downto 0);
    signal Synchronized_Tmp : std_logic;

begin

    -- On attend qu'une entrée soit stable pour changer l'état du signal synchronisé.
    process(rst, clk, UnSynchronized)
    begin
        if rst = '1' then
            Synchronized_Tmp <= UnSynchronized;
            SynchroVect_cur <= (others => UnSynchronized);
        elsif rising_edge(clk) then
            -- SynchroVect <= UnSynchronized & SynchroVect(SampleLength-1 downto 1);
            if Synchronized_Tmp = '1' then
                if signed(SynchroVect_nxt) = 0 then
                    Synchronized_Tmp <= '0';
                end if;
            else
                if signed(SynchroVect_nxt) = -1 then
                    Synchronized_Tmp <= '1';
                end if;
            end if;
            
            SynchroVect_cur <= SynchroVect_nxt;
        end if;
    end process;
    
    Synchronized <= Synchronized_Tmp;
    SynchroVect_nxt <= UnSynchronized & SynchroVect_cur(SampleLength-1 downto 1);
    
end behavior;
