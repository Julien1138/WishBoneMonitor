#ifndef GRAPHTAB_H
#define GRAPHTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "virtualtab.h"

class GraphTab : public VirtualTab
{
    Q_OBJECT
public:
    explicit GraphTab(QWidget *parent = 0);
    ~GraphTab();

//    TabType     GetType(){return eGraphTab;}

signals:

public slots:

private:
//    QVBoxLayout     m_Layout;
//    QwtPlot*        m_pPlot;
//    QwtPlotCurve*   m_pCurve;
//    double*         m_pX;
//    double*         m_pY;

};

#endif // GRAPHTAB_H
