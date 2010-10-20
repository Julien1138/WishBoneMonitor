#ifndef WBWRITEREGISTERDLG_H
#define WBWRITEREGISTERDLG_H

#include "WishBoneWidgetDlg.h"
#include "WBWriteRegisterDoc.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QString>

class WBWriteRegisterDlg : public WishBoneWidgetDlg
{
    Q_OBJECT
public:
    explicit WBWriteRegisterDlg(QList<WishBoneRegister*>* plistRegisters
                     , WBWriteRegisterDoc* pDoc
                     , QWidget *parent = 0);

signals:

public slots:
    void    SetpDoc(int Idx);

private:
    QComboBox   m_ComboRegisterChoice;

    QVBoxLayout m_MainLayout;
    QHBoxLayout m_ButtonsLayout;
};

#endif // WBWRITEREGISTERDLG_H