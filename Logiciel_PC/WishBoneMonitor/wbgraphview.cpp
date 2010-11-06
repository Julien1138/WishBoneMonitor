#include "wbgraphview.h"
#include "qwt_legend.h"

WBGraphView::WBGraphView(WBGraphDoc*  pDoc, WBGraphDlg*  pDlg, QWidget *parent) :
    WishBoneWidgetView(pDoc, pDlg, parent)
{
    m_pLayout = new QGridLayout();

    m_pPlot = new QwtPlot();
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");
    m_pPlot->setEnabled(false);

    ((QGridLayout*) m_pLayout)->addWidget(m_pPlot, 0, 0);

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
}

void WBGraphView::ModeChanged()
{
    m_pPlot->setDisabled(m_pDoc->ConfigMode());

    if (!m_pDoc->ConfigMode())
    {
        ((WBGraphDoc*) m_pDoc)->ResetTables();
    }
}

void WBGraphView::Refresh()
{
    ((WBGraphDoc*) m_pDoc)->UdpateTables();

    for (int i(0) ; i < ((WBGraphDoc*) m_pDoc)->NbrOfCurves() ; i++)
    {
        m_CurveList.value(i)->setData(*(((WBGraphDoc*) m_pDoc)->DateTab(i))
                                    , *(((WBGraphDoc*) m_pDoc)->ValueTab(i)));
    }

    m_pPlot->replot();
}

void WBGraphView::UpdateWidget()
{
    WishBoneWidgetView::UpdateWidget();

    while (!m_CurveList.isEmpty())
    {
        delete m_CurveList.front();
        m_CurveList.pop_front();
    }

    for (int i(0) ; i < ((WBGraphDoc*) m_pDoc)->NbrOfCurves() ; i++)
    {
        QwtPlotCurve* pCurve = new QwtPlotCurve(((WBGraphDoc*) m_pDoc)->CurveName(i));
        pCurve->setStyle(QwtPlotCurve::Lines);
        pCurve->setCurveAttribute(QwtPlotCurve::Inverted);
        pCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
        pCurve->setPen(QColor(Qt::darkGreen));
        pCurve->attach(m_pPlot);
        m_CurveList.push_back(pCurve);

        connect(((WBGraphDoc*) m_pDoc)->Register(i), SIGNAL(UpdateWidget()), this, SLOT(Refresh()));
    }

    if (((WBGraphDoc*) m_pDoc)->NbrOfCurves())
    {
        m_pPlot->setAxisTitle(QwtPlot::yLeft, ((WBGraphDoc*) m_pDoc)->Register(0)->Unit());
    }
}
