#include "graphtab.h"
#include "qwt_legend.h"

GraphTab::GraphTab(QWidget *parent)
    : VirtualTab(parent)
{
//    m_pX = new double[10];
//    m_pX[0] = 0;
//    m_pX[1] = 1;
//    m_pX[2] = 2;
//    m_pX[3] = 3;
//    m_pX[4] = 4;
//    m_pX[5] = 5;
//    m_pX[6] = 6;
//    m_pX[7] = 7;
//    m_pX[8] = 8;
//    m_pX[9] = 9;

//    m_pY = new double[10];
//    m_pY[0] = 0;
//    m_pY[1] = -1;
//    m_pY[2] = 2;
//    m_pY[3] = -3;
//    m_pY[4] = 4;
//    m_pY[5] = -5;
//    m_pY[6] = 6;
//    m_pY[7] = -7;
//    m_pY[8] = 8;
//    m_pY[9] = -9;

//    m_pPlot = new QwtPlot(QwtText("Test"));
//    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
//    m_pPlot->setAxisTitle(QwtPlot::yLeft, "Toto");
//    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");

//    m_pCurve = new QwtPlotCurve("Courbe 1");
//    m_pCurve->setData(m_pX, m_pY, 10);
//    m_pCurve->setStyle(QwtPlotCurve::Lines);
//    m_pCurve->setCurveAttribute(QwtPlotCurve::Fitted);
//    m_pCurve->setPen(QColor(Qt::darkGreen));
//    m_pCurve->attach(m_pPlot);

//    m_Layout.addWidget(m_pPlot);
//    setLayout(&m_Layout);
}

GraphTab::~GraphTab()
{
}
