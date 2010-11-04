#ifndef WBREADREGISTERDLG_H
#define WBREADREGISTERDLG_H

#include "WishBoneWidgetDlg.h"
#include "WBReadRegisterDoc.h"

#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QString>

class WBReadRegisterDlg : public WishBoneWidgetDlg
{
    Q_OBJECT
public:
    explicit WBReadRegisterDlg(QList<WishBoneRegister*>* plistRegisters
                     , WBReadRegisterDoc* pDoc
                     , QWidget *parent = 0);

    void UpdateData();

signals:

public slots:
    void    OnAccept();

private:
    QComboBox   m_ComboRegisterChoice;

    QVBoxLayout m_MainLayout;
    QHBoxLayout m_ButtonsLayout;
};

#endif // WBREADREGISTERDLG_H
