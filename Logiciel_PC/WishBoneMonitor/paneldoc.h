#ifndef PANELDOC_H
#define PANELDOC_H

#include <QString>
#include <QList>
#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"

class PanelDoc
{
public:
    PanelDoc(const QString &Title, bool* pConnectedMode, QList<WishBoneRegister*>* plistRegisters);
    ~PanelDoc();

    QString                     Title(){return m_Title;}
    QList<WishBoneWidgetDoc*>*  GetWidgetList(){return &m_listWidget;}
    bool*                       GetpConnectedMode(){return m_pConnectedMode;}
    QList<WishBoneRegister*>*   GetplistRegisters(){return m_plistRegisters;}

    void    AddWidget(WishBoneWidgetDoc* Widget);

private:
    QString                     m_Title;
    QList<WishBoneWidgetDoc*>   m_listWidget;
    bool*                       m_pConnectedMode;
    QList<WishBoneRegister*>*   m_plistRegisters;
};

#endif // PANELDOC_H
