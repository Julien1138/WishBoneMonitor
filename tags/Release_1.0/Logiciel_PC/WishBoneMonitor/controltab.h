#ifndef CONTROLTAB_H
#define CONTROLTAB_H

#include <QWidget>
#include <QList>
#include <QGridLayout>
#include "virtualtab.h"
#include "PanelDoc.h"
#include "registerdisplay.h"
#include "mailboxdriver.h"

class ControlTab : public VirtualTab
{
    Q_OBJECT
public:
    explicit ControlTab(MailBoxDriver *pMailBox, PanelDoc *pDoc, QWidget *parent = 0);
    ~ControlTab();

    bool UpdateRegisters();
    bool UpdateLayout();
    bool UpdateData();
    bool UpdateButtons();

    TabType     GetType(){return eControlTab;}
    PanelDoc*   GetPanel(){return m_pDoc;}

signals:

public slots:
    void AddRegister();
    void DelRegister(RegisterDisplay* pReg);

private:
    QGridLayout             m_Layout;
    QList<RegisterDisplay*> m_listRegisterDisplay;
    MailBoxDriver*          m_pMailBox;
    PanelDoc*               m_pDoc;

    void contextMenuEvent(QContextMenuEvent * event);

};

#endif // CONTROLTAB_H
