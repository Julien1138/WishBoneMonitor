#ifndef WISHBONEMONITOR_H
#define WISHBONEMONITOR_H

#include "wishboneregister.h"
#include "PanelDoc.h"
#include "MailboxDriver.h"
#include <QList>

class WishBoneMonitor
{
public:
    WishBoneMonitor();
    ~WishBoneMonitor();

    QList<WishBoneRegister*>*   GetRegisterList(){return & m_listRegisters;}
    QList<PanelDoc*>*           GetPanelList(){return & m_listPanel;}
    MailBoxDriver*              GetMailBox(){return m_pMailBox;}
    bool                        ConfigMode(){return !m_ConnectedMode;}

    PanelDoc*   AddPanel(const QString &PanelTitle);
    void        DelPanel(PanelDoc* Panel);
    void        ClearPanelList();
    bool        AddRegister(const WishBoneRegister &Register);
    bool        DelRegister(unsigned long Address);
    bool        RegisterExists(const unsigned long &Address, const bool &Write_nRead) const;
    void        ClearRegisterList();
    void        SetConnectMode(){m_ConnectedMode = true;}
    void        SetConfigMode(){m_ConnectedMode = false;}

private:
    QList<WishBoneRegister*>    m_listRegisters;
    QList<PanelDoc*>            m_listPanel;
    MailBoxDriver*              m_pMailBox;
    bool                        m_ConnectedMode;

    bool    RegisterExists(const WishBoneRegister & Register) const;
    int     RegisterIdx(unsigned long Addr) const;
};

#endif // WISHBONEMONITOR_H
