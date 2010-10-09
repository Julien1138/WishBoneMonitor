----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     wb_Memory_Pack
--
-- Description:        
--
-- 
-- Create Date:     19/07/2009
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.math_real.all;

library wb_Memory_Lib;

package wb_Memory_Pack is

    component wb_DualPortRAM
        generic
        (
            WB_Addr_Width   : integer;
            WB_Data_Width   : integer
        );
        port
        (
            wb_clk_i        : in std_logic;
            wb_rst_i        : in std_logic;
            
        -- Interface A
            wb_we_i_A   : in std_logic := '0';
            wb_adr_i_A  : in std_logic_vector(WB_Addr_Width - 1 downto 0);
            wb_dat_i_A  : in std_logic_vector(WB_Data_Width - 1 downto 0) := (others => '0');
            wb_dat_o_A  : out std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_cyc_i_A  : in std_logic;
            wb_stb_i_A  : in std_logic;
            wb_ack_o_A  : out std_logic;
            
        -- Interface B
            wb_we_i_B  : in std_logic := '0';
            wb_adr_i_B : in std_logic_vector(WB_Addr_Width - 1 downto 0);
            wb_dat_i_B : in std_logic_vector(WB_Data_Width - 1 downto 0) := (others => '0');
            wb_dat_o_B : out std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_cyc_i_B : in std_logic;
            wb_stb_i_B : in std_logic;
            wb_ack_o_B : out std_logic
        );
    end component;
    
    component wb_FIFO
        generic
        (
            FIFOSize        : integer;
            WB_Data_Width   : integer
        );
        port
        (
            wb_clk_i        : in std_logic;
            wb_rst_i        : in std_logic;
            
        -- Input Interface
            wb_we_i_Input   : in std_logic;
            wb_dat_i_Input  : in std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_cyc_i_Input  : in std_logic;
            wb_stb_i_Input  : in std_logic;
            wb_ack_o_Input  : out std_logic;
            wb_ful_o_Input  : out std_logic;
            
        -- Output Interface
            wb_we_i_Output  : in std_logic;
            wb_dat_o_Output : out std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_cyc_i_Output : in std_logic;
            wb_stb_i_Output : in std_logic;
            wb_ack_o_Output : out std_logic;
            wb_emp_o_Output : out std_logic
        );
    end component;
    
    component FIFO
        generic
        (
            FIFOSize        : integer;
            Data_Width      : integer
        );
        port
        (
            rst         : in std_logic;
            clk         : in std_logic;
            
            Write_en    : in std_logic;
            Data_in     : in std_logic_vector(Data_Width - 1 downto 0);
            Read_en     : in std_logic;
            Data_out    : out std_logic_vector(Data_Width - 1 downto 0);
            
            FIFO_Empty  : out std_logic;
            FIFO_Full   : out std_logic
        );
    end component;

end wb_Memory_Pack;
