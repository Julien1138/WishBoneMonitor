#include "wbgraphview.h"
#include "qwt_legend.h"
#include <QSignalMapper>

WBGraphView::WBGraphView(WBGraphDoc*  pDoc, WBGraphDlg*  pDlg, QWidget *parent) :
    WishBoneWidgetView(pDoc, pDlg, parent)
{
    m_pLayout = new QGridLayout();

    m_pPlot = new QwtPlot();
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");
    m_pPlot->setEnabled(false);

    ((QGridLayout*) m_pLayout)->addWidget(m_pPlot, 0, 0);

    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(Refresh()));

    UpdateWidget();

    m_GroupBox.setLayout(m_pLayout);

    setMinimumSize(WBGRAPH_WIDTH_MIN, WBGRAPH_HEIGHT_MIN);

    UpdateDisplay();
}

WBGraphView::~WBGraphView()
{
    while (!m_CurveList.isEmpty())
    {
        delete m_CurveList.front();
        m_CurveList.pop_front();
    }
    delete m_pPlot;
    delete m_Timer;
}

void WBGraphView::ModeChanged()
{
    m_pPlot->setDisabled(m_pDoc->ConfigMode());

    if (!m_pDoc->ConfigMode())
    {
        ((WBGraphDoc*) m_pDoc)->ResetTables();

        m_Timer->start(100);
    }
    else
    {
        m_Timer->stop();
    }
}

void WBGraphView::Refresh()
{
    for (int i(0) ; i < ((WBGraphDoc*) m_pDoc)->NbrOfCurves() ; i++)
    {
        m_CurveList.value(i)->setData(*(((WBGraphDoc*) m_pDoc)->DateTab(i))
                                    , *(((WBGraphDoc*) m_pDoc)->ValueTab(i)));
    }

    double Max(((WBGraphDoc*) m_pDoc)->LatestDate());
    double Min(Max - ((WBGraphDoc*) m_pDoc)->RunningTime());
    m_pPlot->setAxisScale(QwtPlot::xBottom, Min, Max);

    m_pPlot->replot();
}

void WBGraphView::UpdateCurve(int Idx)
{
    ((WBGraphDoc*) m_pDoc)->UdpateTable(Idx);
}

void WBGraphView::UpdateWidget()
{
    WishBoneWidgetView::UpdateWidget();

    while (!m_CurveList.isEmpty())
    {
        delete m_CurveList.front();
        m_CurveList.pop_front();
    }

    QSignalMapper *mapper = new QSignalMapper();

    for (int i(0) ; i < ((WBGraphDoc*) m_pDoc)->NbrOfCurves() ; i++)
    {
        QwtPlotCurve* pCurve = new QwtPlotCurve(((WBGraphDoc*) m_pDoc)->CurveName(i));
        pCurve->setStyle(QwtPlotCurve::Lines);
        pCurve->setCurveAttribute(QwtPlotCurve::Inverted);
        pCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
        pCurve->setPen(CurveColor(i));
        pCurve->attach(m_pPlot);
        m_CurveList.push_back(pCurve);

        connect(((WBGraphDoc*) m_pDoc)->Register(i), SIGNAL(UpdateWidget()), mapper, SLOT(map()));
        mapper->setMapping(((WBGraphDoc*) m_pDoc)->Register(i), i);
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(UpdateCurve(int)));

    if (((WBGraphDoc*) m_pDoc)->NbrOfCurves())
    {
        m_pPlot->setAxisTitle(QwtPlot::yLeft, ((WBGraphDoc*) m_pDoc)->Register(0)->Unit());
    }
}

QColor CurveColor(int Idx)
{
    switch (Idx)
    {
    case 0:
        return Qt::darkGreen;
    case 1:
        return Qt::darkRed;
    case 2:
        return Qt::darkBlue;
    case 3:
        return Qt::darkMagenta;
    case 4:
        return Qt::darkYellow;
    default:
        return Qt::darkGreen;
    }
}
