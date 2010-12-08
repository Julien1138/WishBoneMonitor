----------------------------------------------------------------------------------
-- Engineer:        Julien Aupart
-- 
-- Module Name:     UART
--
-- Description:     Universal Asynchronous Receiver Transmitter
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

entity UART is
    generic
    (
        GlobalClockFrequency    : integer := 50e6;  -- design frequency
        SerializerBaudRate      : integer := 9600;
        SerializerDataBits      : integer := 8;
        SerializerEndianness    : std_logic := '0'; -- 0 => LSB first; 1 => MSB first
        SerializerParityBit     : integer := 1;
        SerializerParity        : std_logic := '0'; -- 0 => even Parity; 1 => odd Parity
        SerializerStopBits      : integer := 2;
        DeserializerBaudRate    : integer := 9600;
        DeserializerDataBits    : integer := 8;
        DeserializerEndianness  : std_logic := '0'; -- 0 => LSB first; 1 => MSB first
        DeserializerParityBit   : integer := 1;
        DeserializerParity      : std_logic := '0'; -- 0 => even Parity; 1 => odd Parity
        DeserializerStopBits    : integer := 2
    );
    port
    (
    -- Global inputs :
        rst                 : in std_logic;         -- global asynchronous reset
        clk                 : in std_logic;         -- global clock
        
    -- Serial interface :
        Rx                  : in std_logic;         -- Rx serial signal
        Tx                  : out std_logic;        -- Tx serial signal
        
    -- DATA interface :
        SendData            : in std_logic;
        DataToSend          : in std_logic_vector(SerializerDataBits - 1 downto 0);
        SerializerBusy      : out std_logic;
        ParityError         : out std_logic;
        DataAvailable       : out std_logic;
        ReceivedData        : out std_logic_vector(DeserializerDataBits - 1 downto 0)
    );
end UART;

architecture behavior of UART is
    
    signal WaitForStartBit          : std_logic;
    signal SerializerTopBit         : std_logic;
    signal SerializerStart          : std_logic;
    signal SerializerData           : std_logic_vector(SerializerDataBits - 1 downto 0);
    signal SerializerBusy_tmp       : std_logic;
    
    signal DeserialiserBusy         : std_logic;
    signal DeserialiserBusy_prev    : std_logic;
    signal DeserializerTopBit       : std_logic;
    signal DeserializerStart        : std_logic;
    
    signal Tx_tmp                   : std_logic;
    
begin

-- Serializer

    Serializer_start_sending_process : process(rst, clk)
    begin
        if rst = '1' then
            WaitForStartBit <= '1';
            SerializerStart <= '0';
            SerializerData <= (others => '0');
        elsif rising_edge(clk) then
            SerializerStart <= '0';
            
            if WaitForStartBit = '1' and SendData = '1' then
                SerializerStart <= '1';
                SerializerData <= DataToSend;
                WaitForStartBit <= '0';
            end if;
            
            if SendData = '0' then
                WaitForStartBit <= '1';
            end if;
        end if;
    end process;

    UART_Sequencer_Serializer_inst : UART_Sequencer
    generic map
    (
        GlobalClockFrequency => GlobalClockFrequency,
        Ser_nDes => '1',
        BaudRate => SerializerBaudRate,
        DataBits => SerializerDataBits,
        ParityBit => SerializerParityBit,
        StopBits => SerializerStopBits
    )
    port map
    (
        rst => rst,
        clk => clk,
        Start => SerializerStart,
        TopBit => SerializerTopBit,
        Busy => SerializerBusy_tmp
    );
    SerializerBusy <= SerializerBusy_tmp;

    UART_Serializer_inst : UART_Serializer
    generic map
    (
        DataBits => SerializerDataBits,
        Endianness => SerializerEndianness,
        ParityBit => SerializerParityBit,
        Parity => SerializerParity,
        StopBits => SerializerStopBits
    )
    port map
    (
        rst => rst,
        clk => clk,
        TopBit => SerializerTopBit,
        Tx => Tx_tmp,
        LoadData => SerializerStart,
        DataIn => SerializerData
    );
    Tx <= 'Z' when rst = '1' else
          Tx_tmp when SerializerBusy_tmp = '1' else
          '1';
    
-- Deserializer

    Deserializer_Receive_Flag_process : process(rst, clk)
    begin
        if rst = '1' then
            DataAvailable <= '0';
            DeserialiserBusy_prev <= '0';
        elsif rising_edge(clk) then
            DataAvailable <= '0';
            
            if DeserialiserBusy = '0' and DeserialiserBusy_prev = '1' then
                DataAvailable <= '1';
            end if;
            
            DeserialiserBusy_prev <= DeserialiserBusy;
        end if;
    end process;
    
    UART_StartBitDetector_inst : UART_StartBitDetector
    port map
    (
        rst => rst,
        clk => clk,
        Rx => Rx,
        WaitForStartBit => "not" (DeserialiserBusy),
        StartBitDetected => DeserializerStart
    );

    UART_Sequencer_Deserializer_inst : UART_Sequencer
    generic map
    (
        GlobalClockFrequency => GlobalClockFrequency,
        Ser_nDes => '0',
        BaudRate => DeserializerBaudRate,
        DataBits => DeserializerDataBits,
        ParityBit => DeserializerParityBit,
        StopBits => DeserializerStopBits
    )
    port map
    (
        rst => rst,
        clk => clk,
        Start => DeserializerStart,
        TopBit => DeserializerTopBit,
        Busy => DeserialiserBusy
    );
    
    UART_Deserializer_inst : UART_Deserializer
    generic map
    (
        DataBits => DeserializerDataBits,
        Endianness => DeserializerEndianness,
        ParityBit => DeserializerParityBit,
        Parity => DeserializerParity,
        StopBits => DeserializerStopBits
    )
    port map
    (
        rst => rst,
        clk => clk,
        TopBit => DeserializerTopBit,
        Busy => DeserialiserBusy,
        Rx => Rx,
        ParityError => ParityError,
        DataOut => ReceivedData
    );

end behavior;