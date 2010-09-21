#ifndef GRAPHTAB_H
#define GRAPHTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include "virtualtab.h"
#include "graphdisplay.h"
#include "PanelDoc.h"
#include "mailboxdriver.h"

class GraphTab : public VirtualTab
{
    Q_OBJECT
public:
    explicit GraphTab(MailBoxDriver *pMailBox, PanelDoc *pDoc, QWidget *parent = 0);
    ~GraphTab();

    bool UpdateRegisters();
    bool UpdateLayout();
    bool UpdateData();
    bool UpdateButtons();

    TabType     GetType(){return eGraphTab;}
    PanelDoc*   GetPanel(){return m_pDoc;}

signals:

public slots:
    void AddCurve();
    void AddGraph();
    void StartAll();
    void StopAll();

private:
    QVBoxLayout             m_Layout;

    QHBoxLayout             m_TopLayout;
    QPushButton             m_StartAllButton;
    QPushButton             m_StopAllButton;
    QLabel                  m_PeriodeLabel;
    QLineEdit               m_PeriodeEdit;
    QLabel                  m_msLabel;

    QVBoxLayout             m_GraphsLayout;

    QList<QwtPlot*>         m_listPlot;
    QList<int>              m_listNbrOfCurves;
    QList<QwtPlotCurve*>    m_listCurve;

    MailBoxDriver*          m_pMailBox;
    PanelDoc*               m_pDoc;

    void contextMenuEvent(QContextMenuEvent * event);

};

#endif // GRAPHTAB_H
