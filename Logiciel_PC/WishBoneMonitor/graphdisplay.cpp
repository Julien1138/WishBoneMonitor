#include "graphdisplay.h"
#include "qwt_legend.h"
#include <QContextMenuEvent>
#include <QMenu>

GraphDisplay::GraphDisplay(QWidget *parent) :
    QWidget(parent),
    m_GroupBox(this)
{
    m_pX = new double[10];
    m_pX[0] = 0;
    m_pX[1] = 1;
    m_pX[2] = 2;
    m_pX[3] = 3;
    m_pX[4] = 4;
    m_pX[5] = 5;
    m_pX[6] = 6;
    m_pX[7] = 7;
    m_pX[8] = 8;
    m_pX[9] = 9;

    m_pY = new double[10];
    m_pY[0] = 0;
    m_pY[1] = -1;
    m_pY[2] = 2;
    m_pY[3] = -3;
    m_pY[4] = 4;
    m_pY[5] = -5;
    m_pY[6] = 6;
    m_pY[7] = -7;
    m_pY[8] = 8;
    m_pY[9] = -9;

    m_pPlot = new QwtPlot(QwtText("Test"));
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    m_pPlot->setAxisTitle(QwtPlot::yLeft, "Toto");
    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");

    m_pCurve = new QwtPlotCurve("Courbe 1");
    m_pCurve->setData(m_pX, m_pY, 10);
    m_pCurve->setStyle(QwtPlotCurve::Lines);
    m_pCurve->setCurveAttribute(QwtPlotCurve::Fitted);
    m_pCurve->setPen(QColor(Qt::darkGreen));
    m_pCurve->attach(m_pPlot);

    m_Layout.addWidget(m_pPlot);
    m_GroupBox.setLayout(&m_Layout);
    m_GroupBox.setGeometry(0, 0, width(), height());
}

void GraphDisplay::UpdateDisplay(QRect Rect)
{
    m_GroupBox.setGeometry(Rect);
}

void GraphDisplay::contextMenuEvent(QContextMenuEvent * event)
{
    if(event->x() > 0 && event->x() < width() &&
       event->y() > 0 && event->y() < height())
    {
        QMenu * menu = new QMenu(this);
        m_pDeleteAction = menu->addAction("Ajouter Courbe");
        m_pDeleteAction = menu->addAction("Supprimer Graphe", this, SLOT(Delete()));
        m_pDeleteAction = menu->addAction("Ajouter Graphe");

        menu->exec(event->globalPos());
    }
}

void GraphDisplay::Delete()
{
    emit Delete(this);
}
