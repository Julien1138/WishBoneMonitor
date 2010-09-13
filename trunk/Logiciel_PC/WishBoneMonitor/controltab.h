#ifndef CONTROLTAB_H
#define CONTROLTAB_H

#include <QWidget>
#include <QList>
#include <QGridLayout>
#include "wishbonemonitor.h"
#include "registerdisplay.h"

class ControlTab : public QWidget
{
    Q_OBJECT
public:
    explicit ControlTab(WishBoneMonitor *pDoc, QWidget *parent = 0);
    ~ControlTab();

    bool UpdateRegisters();
    bool UpdateLayout();
    bool UpdateData();
    bool UpdateButtons();

signals:

public slots:

private:
    QGridLayout             m_Layout;
    QList<RegisterDisplay*> m_listRegisterDisplay;
    WishBoneMonitor*        m_pDoc;

};

#endif // CONTROLTAB_H
