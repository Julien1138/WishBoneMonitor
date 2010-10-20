#ifndef WISHBONEWIDGETDLG_H
#define WISHBONEWIDGETDLG_H

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>

#include "WishBoneMonitor.h"
#include "WishBoneWidgetDoc.h"

class WishBoneWidgetDlg : public QDialog
{
    Q_OBJECT
public:
    explicit WishBoneWidgetDlg(QList<WishBoneRegister*>* plistRegisters
                             , WishBoneWidgetDoc* pDoc
                             , QWidget *parent);

signals:

public slots:
    void    SetTitle(QString Title);

protected:
    QList<WishBoneRegister*>*   m_plistRegisters;
    WishBoneWidgetDoc*          m_pDoc;

    QLayout*    m_pLayout;
    QLineEdit   m_EditTitle;
    QPushButton m_OKButton;
    QPushButton m_CancelButton;
};

#endif // WISHBONEWIDGETDLG_H
