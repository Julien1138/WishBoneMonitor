#ifndef WBWRITEREGISTERVIEW_H
#define WBWRITEREGISTERVIEW_H

#include "WishBoneWidgetView.h"
#include "WBWriteRegisterDoc.h"
#include "WBWriteRegisterDlg.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class WBWriteRegisterView : public WishBoneWidgetView
{
    Q_OBJECT
public:
    WBWriteRegisterView(WBWriteRegisterDoc*  pDoc, WBWriteRegisterDlg*  pDlg, QWidget *parent = 0);

signals:

public slots:
    void    ModeChanged();
    void    WriteRegister();
    void    Refresh();

private:
    QLineEdit           m_EditValue;
    QLabel              m_LabelUnit;
    QPushButton*        m_pSetButton;

    void    UpdateWidget();

};

#endif // WBWRITEREGISTERVIEW_H
