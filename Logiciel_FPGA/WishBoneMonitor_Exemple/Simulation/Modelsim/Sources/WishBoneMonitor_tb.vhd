library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

library UART_LIB;
use UART_LIB.UART_PACK.all;

entity WishBoneMonitor_tb is
end WishBoneMonitor_tb;

architecture behavior of WishBoneMonitor_tb is

    signal wb_we_i         : std_logic := '0';
    signal wb_dat_i        : std_logic_vector(7 downto 0) := (others => '0');
    signal wb_dat_o        : std_logic_vector(7 downto 0);
    signal wb_cyc_i        : std_logic := '0';
    signal wb_stb_i        : std_logic := '0';
    signal wb_ack_o        : std_logic;
    signal wb_rty_o        : std_logic;
    signal wb_wait_o       : std_logic;

    component WishBoneMonitor
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
    end component;

    signal rst                 : std_logic := '1';
    signal clk                 : std_logic := '1';
    signal Rx                  : std_logic := '1';
    signal Tx                  : std_logic;
    signal Led                 : std_logic_vector(7 downto 0);
    signal Slider              : std_logic_vector(7 downto 0) := "01100110";
        
    constant clk_period : time := 20 ns;    -- 50 MHz

begin

    rst <= '0' after 53 ns;
    clk <= not clk after clk_period/2;
    
    process
    begin
        wait for 100 ns;
        wb_we_i <= '1';
        wb_dat_i <= X"01";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"05";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"04";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for 100 ns;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait for clk_period;
        wb_we_i <= '1';
        wb_dat_i <= X"00";
        wb_cyc_i <= '1';
        wb_stb_i <= '1';
        wait for 2*clk_period;
        wb_we_i <= '0';
        wb_cyc_i <= '0';
        wb_stb_i <= '0';
        
        wait;
    end process;
    
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
        wb_we_i => wb_we_i,
        wb_dat_i => wb_dat_i,
        wb_dat_o => wb_dat_o,
        wb_cyc_i => wb_cyc_i,
        wb_stb_i => wb_stb_i,
        wb_ack_o => wb_ack_o,
        wb_rty_o => wb_rty_o,
        wb_wait_o => wb_wait_o,
        Rx => Tx,
        Tx => Rx
    );
    
    WishBoneMonitor_tb : WishBoneMonitor
    port map
    (
        rst => rst,
        clk => clk,
        Rx => Rx,
        Tx => Tx,
        Led => Led,
        Slider => Slider
    );

end behavior;
