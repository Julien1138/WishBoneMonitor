#ifndef WISHBONEMONITOR_H
#define WISHBONEMONITOR_H

#include "wishboneregister.h"
#include "PanelDoc.h"
#include "MailboxDriver.h"
#include <QList>
#include <QSettings>

class WishBoneMonitor
{
public:
    WishBoneMonitor();
    ~WishBoneMonitor();

    void    Save(QSettings* pSettings);

    QList<WishBoneRegister*>*   GetRegisterList(){return & m_listRegisters;}
    QList<PanelDoc*>*           GetPanelList(){return & m_listPanel;}
    MailBoxDriver*              GetMailBox(){return m_pMailBox;}

    PanelDoc*   AddPanel(const QString &PanelTitle);
    void        DelPanel(PanelDoc* Panel);
    void        ClearPanelList();
    bool        AddRegister(const WishBoneRegister &Register);
    bool        DelRegister(const unsigned long &Address, const bool &Write_nRead);
    void        DelRegister(const int Idx);
    bool        RegisterExists(const unsigned long &Address, const bool &Write_nRead) const;
    int         RegisterIdx(const unsigned long &Address, const bool &Write_nRead) const;
    void        ClearRegisterList();
    bool        ConfigMode(){return !(m_pMailBox->IsConnected());}

private:
    QList<WishBoneRegister*>    m_listRegisters;
    QList<PanelDoc*>            m_listPanel;
    MailBoxDriver*              m_pMailBox;

    bool        RegisterExists(const WishBoneRegister & Register) const;
};

#endif // WISHBONEMONITOR_H
