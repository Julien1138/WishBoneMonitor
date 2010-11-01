#ifndef MAILBOXDRIVER_H
#define MAILBOXDRIVER_H
#include "QextSerialBase/qextserialport.h"
#include "wishboneregister.h"

class MailBoxDriver
{
public:
    MailBoxDriver();
    ~MailBoxDriver();

    bool Connect(QString PortName);
    void Disconnect();
    bool IsConnected();
    void ResetMailBox();

    QextSerialPort* GetPort(){return m_pPort;}

    void SendRegister(WishBoneRegister* Reg);
    bool DecodeRegister(WishBoneRegister* Reg);
    bool DataAvailable();

private:
    PortSettings*   m_pSettings;
    QextSerialPort* m_pPort;

private:
    char Checksum(char* Data, int Length);
};

#endif // MAILBOXDRIVER_H
