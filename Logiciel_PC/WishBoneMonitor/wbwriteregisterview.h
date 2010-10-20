#ifndef WBWRITEREGISTERVIEW_H
#define WBWRITEREGISTERVIEW_H

#include "WishBoneWidgetView.h"
#include "WBWriteRegisterDoc.h"

#include <QLineEdit>
#include <QPushButton>

class WBWriteRegisterView : public WishBoneWidgetView
{
    Q_OBJECT
public:
    WBWriteRegisterView(WBWriteRegisterDoc*  pDoc, QWidget *parent = 0);

signals:

public slots:
    void    ModeChanged();
    void    WriteRegister();

private:
    QLineEdit           m_EditValue;
    QPushButton         m_SetButton;
};

#endif // WBWRITEREGISTERVIEW_H
