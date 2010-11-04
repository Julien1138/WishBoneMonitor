#include "wbgraphview.h"
#include "qwt_legend.h"

WBGraphView::WBGraphView(WBGraphDoc*  pDoc, WBGraphDlg*  pDlg, QWidget *parent) :
    WishBoneWidgetView(pDoc, pDlg, parent)
{
    m_pLayout = new QGridLayout();

    m_pPlot = new QwtPlot();
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");

    m_pLayout->addWidget(m_pPlot);

    UpdateWidget();

    m_GroupBox.setLayout(m_pLayout);

    setMinimumSize(WBGRAPH_WIDTH_MIN, WBGRAPH_HEIGHT_MIN);

    UpdateDisplay();
}

WBGraphView::~WBGraphView()
{
    delete m_pPlot;
}

void WBGraphView::ModeChanged()
{
}

void WBGraphView::Refresh()
{
}

void WBGraphView::UpdateWidget()
{
    WishBoneWidgetView::UpdateWidget();
}
