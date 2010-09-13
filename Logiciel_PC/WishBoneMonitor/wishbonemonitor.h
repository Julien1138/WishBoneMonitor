#ifndef WISHBONEMONITOR_H
#define WISHBONEMONITOR_H

#include "wishboneregister.h"
#include "mailboxdriver.h"
#include <QList>

class WishBoneMonitor
{
public:
    WishBoneMonitor();
    ~WishBoneMonitor();

    QList<WishBoneRegister*>*   GetWishBoneRegisterList(){return & m_listWishBoneRegister;}
    MailBoxDriver*              GetMailBox(){return m_pMailBox;}

    void    AddRegister(WishBoneRegister* Reg);
    void    ClearList();
    int     FindRegisters(unsigned long Address, int* IdxTab);

private:
    QList<WishBoneRegister*>    m_listWishBoneRegister;
    MailBoxDriver*              m_pMailBox;
};

#endif // WISHBONEMONITOR_H
