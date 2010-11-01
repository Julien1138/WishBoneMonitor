#ifndef WBREADREGISTERVIEW_H
#define WBREADREGISTERVIEW_H

#include "WishBoneWidgetView.h"
#include "WBReadRegisterDoc.h"
#include "WBReadRegisterDlg.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class WBReadRegisterView : public WishBoneWidgetView
{
    Q_OBJECT
public:
    WBReadRegisterView(WBReadRegisterDoc*  pDoc, WBReadRegisterDlg*  pDlg, QWidget *parent = 0);

signals:

public slots:
    void    ModeChanged();
    void    ReadRegister();
    void    Refresh();

private:
    QLineEdit           m_EditValue;
    QLabel              m_LabelUnit;
    QPushButton*        m_pSetButton;

    void    UpdateWidget();

};

#endif // WBREADREGISTERVIEW_H
