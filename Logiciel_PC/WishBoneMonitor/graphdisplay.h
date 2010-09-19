#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QList>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "wishboneregister.h"
#include "PanelDoc.h"

class GraphDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GraphDisplay(PanelDoc *pDoc, QWidget *parent = 0);
    ~GraphDisplay();

    void UpdateDisplay(QRect);

    QList<WishBoneRegister*>*   GetpRegisterList(){return &m_RegisterList;}
    QList<QwtPlotCurve*>*       GetpCurveList(){return &m_CurveList;}
    QwtPlot*                    GetPlot(){return m_pPlot;}

signals:
    void Delete(GraphDisplay* pReg);
//    void AddNew();

public slots:
    void Delete();
    void AddCurve();
//    void AddOther();

private:
    void contextMenuEvent(QContextMenuEvent * event);
    QAction*        m_pDeleteAction;
    QAction*        m_pAddCurveAction;
    QAction*        m_pAddGraphAction;

    QGroupBox       m_GroupBox;
    QGridLayout     m_Layout;

    QwtPlot*                    m_pPlot;
    PanelDoc*                   m_pDoc;
    QList<WishBoneRegister*>    m_RegisterList;
    QList<QwtPlotCurve*>        m_CurveList;

};

#endif // GRAPHDISPLAY_H
