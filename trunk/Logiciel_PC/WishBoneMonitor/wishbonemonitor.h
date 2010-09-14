#ifndef WISHBONEMONITOR_H
#define WISHBONEMONITOR_H

#include "PanelDoc.h"
#include "MailboxDriver.h"
#include <QList>

class WishBoneMonitor
{
public:
    WishBoneMonitor();
    ~WishBoneMonitor();

    QList<PanelDoc*>*       GetPanelList(){return & m_listPanel;}
    MailBoxDriver*          GetMailBox(){return m_pMailBox;}

    void    AddPanel(PanelDoc* Panel);
    void    ClearList();

private:
    QList<PanelDoc*>        m_listPanel;
    MailBoxDriver*          m_pMailBox;
};

#endif // WISHBONEMONITOR_H
