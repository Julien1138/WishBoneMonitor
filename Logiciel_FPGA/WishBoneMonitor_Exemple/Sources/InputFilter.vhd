----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     InputFilter
--
-- Description:
--   Filtrage des entrées. La sortie change d'état lorsque l'entrée
-- à été stable pendant toute la durée du filtre.
-- 
-- Create Date:     05/12/2010
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity InputFilter is
    generic
    (
        Length      : integer := 20  --! Nombre de coups d'horloge sur lequel est réalisé le filtrage
    );
    port
    (
        clk         : in std_logic;
        rst         : in std_logic;
        
        Input       : in std_logic;
        Output      : out std_logic
    );
end InputFilter;

architecture InputFilter_Behavior of InputFilter is
    
    signal Output_Vector_Cur    : std_logic_vector(Length - 2 downto 0);
    signal Output_Vector_Nxt    : std_logic_vector(Output_Vector_Cur'range);

begin
    
    Output_Vector_Nxt <= Output_Vector_Cur(Output_Vector_Nxt'high-1 downto 0) & Input;
    process(rst, clk)
    begin
        if rst = '1' then
            Output_Vector_Cur <= (others => '1');
            Output <= '1';
        elsif rising_edge(clk) then
        
            if signed(Output_Vector_Cur) = -1 then
                Output <= '1';
            end if;
            if signed(Output_Vector_Cur) = 0 then
                Output <= '0';
            end if;
        
            Output_Vector_Cur <= Output_Vector_Nxt;
            
        end if;
    end process;
    
end InputFilter_Behavior;
