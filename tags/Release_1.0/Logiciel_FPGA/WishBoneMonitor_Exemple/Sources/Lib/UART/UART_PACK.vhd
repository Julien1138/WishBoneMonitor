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

package UART_PACK is

    component UART
        generic
        (
            GlobalClockFrequency    : integer;  -- design frequency
            SerializerBaudRate      : integer;
            SerializerDataBits      : integer;
            SerializerEndianness    : std_logic; -- 0 => LSB first; 1 => MSB first
            SerializerParityBit     : integer;
            SerializerParity        : std_logic; -- 0 => even Parity; 1 => odd Parity
            SerializerStopBits      : integer;
            DeserializerBaudRate    : integer;
            DeserializerDataBits    : integer;
            DeserializerEndianness  : std_logic; -- 0 => LSB first; 1 => MSB first
            DeserializerParityBit   : integer;
            DeserializerParity      : std_logic; -- 0 => even Parity; 1 => odd Parity
            DeserializerStopBits    : integer
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
    end component;

    component UART_Deserializer
        generic
        (
            DataBits        : integer;
            Endianness      : std_logic;    -- 0 => LSB first; 1 => MSB first
            ParityBit       : integer;
            Parity          : std_logic; -- 0 => even Parity; 1 => odd Parity
            StopBits        : integer
        );
        port
        (
        -- Global inputs :
            rst             : in std_logic;        -- global asynchronous reset (active low)
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
    end component;

    component UART_Serializer
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
            rst             : in std_logic;        -- global asynchronous reset (active low)
            clk             : in std_logic;        -- global clock
            
        -- Sequencer interface :
            TopBit          : in std_logic;
            
        -- Serial interface :
            Tx              : out std_logic;    -- Tx serial signal
            
        -- DATA interface :
            LoadData        : in std_logic;
            DataIn          : in std_logic_vector(DataBits - 1 downto 0)
        );
    end component;

    component UART_Sequencer
        generic
        (
            GlobalClockFrequency    : integer;      -- design frequency
            Ser_nDes                : std_logic;    -- 1 => Serializer ; 0 => Deserializer
            BaudRate                : integer;
            DataBits                : integer;
            ParityBit               : integer;
            StopBits                : integer
        );
        port
        (
        -- Global inputs :
            rst         : in std_logic;        -- Global asynchronous reset (active low)
            clk         : in std_logic;        -- Global clock
        
        -- UART interface :
            Start       : in std_logic;
            TopBit      : out std_logic;
            Busy        : out std_logic
        );
    end component;

    component UART_StartBitDetector
        port
        (
        -- Global inputs :
            rst                 : in std_logic;        -- global asynchronous reset (active low)
            clk                 : in std_logic;        -- global clock
            
        -- Serial interface :
            Rx                  : in std_logic;        -- Rx serial signal
            
        -- Sequencer interface :
            WaitForStartBit     : in std_logic;
            StartBitDetected    : out std_logic
        );
    end component;
    
    component UART_FIFO
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
    
    component UART_wb
        generic
        (
            GlobalClockFrequency    : integer;  -- design frequency
            BaudRate                : integer;
            DataBits                : integer;
            Endianness              : std_logic; -- 0 => LSB first; 1 => MSB first
            ParityBit               : integer;
            Parity                  : std_logic; -- 0 => even Parity; 1 => odd Parity
            StopBits                : integer;
            BufferSize              : integer;
            WB_Data_Width           : integer
        );
        port
        (
        -- WishBone interface
            wb_clk_i        : in std_logic;
            wb_rst_i        : in std_logic;
            
            wb_we_i         : in std_logic;
            wb_dat_i        : in std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_dat_o        : out std_logic_vector(WB_Data_Width - 1 downto 0);
            wb_cyc_i        : in std_logic;
            wb_stb_i        : in std_logic;
            wb_ack_o        : out std_logic;
            wb_rty_o        : out std_logic;
            
            wb_wait_o       : out std_logic;    -- Available data waiting
            
        -- Serial interface :
            Rx              : in std_logic;
            Tx              : out std_logic
        );
    end component;
    
end UART_PACK;