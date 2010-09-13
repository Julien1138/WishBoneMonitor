#include "mailboxdriver.h"

#define HEADER_WRITE    0x01
#define HEADER_READ     0x00
#define HEADER_RECEIVE  0x55

MailBoxDriver::MailBoxDriver()
    : m_pPort(NULL)
{
    m_pSettings = new PortSettings();
    m_pSettings->BaudRate = BAUD115200;
    m_pSettings->DataBits = DATA_8;
    m_pSettings->FlowControl = FLOW_OFF;
    m_pSettings->Parity = PAR_NONE;
    m_pSettings->StopBits = STOP_2;
}

MailBoxDriver::~MailBoxDriver()
{
    if (m_pPort != NULL)
    {
        m_pPort->close();
        delete m_pPort;
        m_pPort = NULL;
    }
    delete m_pSettings;
}

bool MailBoxDriver::Connect(QString PortName)
{
    if (m_pPort != NULL)
    {
        m_pPort->close();
        delete m_pPort;
    }
    m_pPort = new QextSerialPort(PortName, *m_pSettings, QextSerialPort::EventDriven);
    m_pPort->open(QIODevice::ReadWrite);
    m_pPort->setBaudRate(BAUD115200);
    m_pPort->setFlowControl(FLOW_OFF);
    m_pPort->setParity(PAR_NONE);
    m_pPort->setDataBits(DATA_8);
    m_pPort->setStopBits(STOP_2);
    if (!(m_pPort->lineStatus() & LS_DSR))
    {
        return false;
    }
    return true;
}

void MailBoxDriver::Disconnect()
{
    if (m_pPort != NULL)
    {
        m_pPort->close();
        delete m_pPort;
        m_pPort = NULL;
    }
}

bool MailBoxDriver::IsConnected()
{
    if (m_pPort == NULL)
        return false;

    if (m_pPort->lineStatus() & LS_DSR)
    {
        return true;
    }
    return false;
}

void MailBoxDriver::SendRegister(WishBoneRegister* Reg)
{
    char Data[10];

    Data[0] = (Reg->Write_nRead() ? HEADER_WRITE : HEADER_READ);
    Data[1] = (char) (Reg->Address() >> 8);
    Data[2] = (char) (Reg->Address());
    Data[3] = (char) (Reg->Value() >> 24);
    Data[4] = (char) (Reg->Value() >> 16);
    Data[5] = (char) (Reg->Value() >> 8);
    Data[6] = (char) (Reg->Value());
    Data[7] = (char) (Reg->Period() >> 8);
    Data[8] = (char) (Reg->Period());
    Data[9] = Checksum(Data, 9);

    m_pPort->write(Data, 10);
}

bool MailBoxDriver::DecodeRegister(WishBoneRegister* Reg)
{
    char Data[10];

    while (1)
    {
        if (m_pPort->bytesAvailable() < 10)
            return false;

        m_pPort->peek(Data, 1);
        if(Data[0] == HEADER_RECEIVE)
        {
            break;
        }
        else
        {
            m_pPort->read(Data, 1);
        }
    }


    m_pPort->read(Data, 10);

    if ((Data[0] == HEADER_RECEIVE) && Checksum(Data, 10) == 0)
    {
        Reg->SetAddress((Data[1] << 8) +
                         Data[2]);

        Reg->SetValue((Data[3] << 24) +
                      (Data[4] << 16) +
                      (Data[5] << 8) +
                       Data[6]);

        Reg->SetDate((Data[7] << 8) +
                      Data[8]);

        return true;
    }
    else
    {
        return false;
    }
}

bool MailBoxDriver::DataAvailable()
{
    return (m_pPort->bytesAvailable() ? true : false);
}

char MailBoxDriver::Checksum(char* Data, int Length)
{
    char CheckSum(0);

    for (int i(0) ; i < Length ; i++)
    {
        CheckSum ^= Data[i];
    }
    return CheckSum;
}
