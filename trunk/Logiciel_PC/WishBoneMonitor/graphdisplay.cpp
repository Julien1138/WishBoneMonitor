#include "graphdisplay.h"
#include "qwt_legend.h"
#include <QContextMenuEvent>
#include <QMenu>
#include "addregisterdialog.h"

GraphDisplay::GraphDisplay(PanelDoc *pDoc, QWidget *parent) :
    QWidget(parent),
    m_GroupBox(this),
    m_pDoc(pDoc)
{
    m_pPlot = new QwtPlot();
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    m_pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");

    m_Layout.addWidget(m_pPlot);
    m_GroupBox.setLayout(&m_Layout);
}

GraphDisplay::~GraphDisplay()
{
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
        m_pAddCurveAction = menu->addAction("Ajouter Courbe", this, SLOT(AddCurve()));
        m_pDeleteAction = menu->addAction("Supprimer Graphe", this, SLOT(Delete()));
        //m_pAddGraphAction = menu->addAction("Ajouter Graphe", this, SLOT(AddOther()));

        menu->exec(event->globalPos());
    }
}

void GraphDisplay::Delete()
{
    emit Delete(this);
}

void GraphDisplay::AddCurve()
{
    AddRegisterDialog Dlg(false, this);
    Dlg.setModal(true);
    if (Dlg.exec() == QDialog::Accepted)
    {
        WishBoneRegister* pReg = new WishBoneRegister(Dlg.Name(),
                                                      Dlg.Address(),
                                                      Dlg.ValueMin(),
                                                      Dlg.ValueMax(),
                                                      Dlg.Signed(),
                                                      Dlg.Unit(),
                                                      Dlg.Write_nRead());
        m_RegisterList.push_back(pReg);
        m_pDoc->AddRegister(pReg);

        QwtPlotCurve* pCurve = new QwtPlotCurve(Dlg.Name());
        pCurve->setStyle(QwtPlotCurve::Lines);
        pCurve->setCurveAttribute(QwtPlotCurve::Inverted);
        pCurve->setPen(QColor(Qt::darkGreen));
        pCurve->attach(m_pPlot);
        m_CurveList.push_back(pCurve);

        m_pPlot->setAxisTitle(QwtPlot::yLeft, Dlg.Unit());
    }
}

//void GraphDisplay::AddOther()
//{
//    emit AddNew();
//}
