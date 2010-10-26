#ifndef PANELDOC_H
#define PANELDOC_H

#include <QString>
#include <QList>
#include <QSettings>
#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"
#include "MailBoxDriver.h"

class PanelDoc
{
public:
    PanelDoc(const QString &Title, MailBoxDriver* pMailBox, QList<WishBoneRegister*>* plistRegisters);
    ~PanelDoc();

    void    Save(QSettings* pSettings);

    QString                     Title(){return m_Title;}
    QList<WishBoneWidgetDoc*>*  GetWidgetList(){return &m_listWidget;}
    MailBoxDriver*              GetMailBox(){return m_pMailBox;}
    QList<WishBoneRegister*>*   GetRegistersList(){return m_plistRegisters;}

    void    AddWidget(WishBoneWidgetDoc* Widget);

private:
    QString                     m_Title;
    QList<WishBoneWidgetDoc*>   m_listWidget;
    MailBoxDriver*              m_pMailBox;
    QList<WishBoneRegister*>*   m_plistRegisters;
};

#endif // PANELDOC_H
