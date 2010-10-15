#ifndef PANELDOC_H
#define PANELDOC_H

#include <QString>
#include <QList>
#include "WishBoneWidgetDoc.h"

class PanelDoc
{
public:
    PanelDoc(const QString &Title, bool* pConnectedMode);
    ~PanelDoc();

    QString                     Title(){return m_Title;}
    QList<WishBoneWidgetDoc*>*  GetWidgetList(){return &m_listWidget;}

    WishBoneWidgetDoc*  AddWidget();

private:
    QString                     m_Title;
    QList<WishBoneWidgetDoc*>   m_listWidget;
    bool*                       m_pConnectedMode;
};

#endif // PANELDOC_H
