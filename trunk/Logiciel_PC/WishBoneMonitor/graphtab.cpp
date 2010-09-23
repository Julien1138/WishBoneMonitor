#include "graphtab.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QRect>
#include "qwt_legend.h"
#include "AddRegisterDialog.h"

static int AddCurveToPlot;

GraphTab::GraphTab(MailBoxDriver *pMailBox, PanelDoc *pDoc, QWidget *parent)
    : VirtualTab(parent)
    , m_pMailBox(pMailBox)
    , m_pDoc(pDoc)
{
    m_StartAllButton.setText("Start All");
    m_StartAllButton.setFixedSize(60, 20);
    m_StartAllButton.setEnabled(false);
    connect(&m_StartAllButton, SIGNAL(clicked()), this, SLOT(StartAll()));
    m_TopLayout.addWidget(&m_StartAllButton);
    m_StopAllButton.setText("Stop All");
    m_StopAllButton.setFixedSize(60, 20);
    m_StopAllButton.setEnabled(false);
    connect(&m_StopAllButton, SIGNAL(clicked()), this, SLOT(StopAll()));
    m_TopLayout.addWidget(&m_StopAllButton);
    m_PeriodeLabel.setText("Periode d'échantillonnage :");
    m_PeriodeLabel.setFixedHeight(20);
    m_TopLayout.addWidget(&m_PeriodeLabel);
    m_PeriodeEdit.setText("100");
    m_PeriodeEdit.setFixedSize(60, 20);
    m_TopLayout.addWidget(&m_PeriodeEdit);
    m_msLabel.setText("ms");
    m_msLabel.setFixedHeight(20);
    m_TopLayout.addWidget(&m_msLabel);
    m_TopLayout.addStretch();

    m_Layout.addLayout(&m_TopLayout);
    m_Layout.addLayout(&m_GraphsLayout);

    setLayout(&m_Layout);
}

GraphTab::~GraphTab()
{
    while (!m_listPlot.empty())
    {
        delete m_listPlot.front();
        m_listPlot.pop_front();
    }
}

bool GraphTab::UpdateRegisters()
{
    while (!m_listCurve.empty())
    {
        delete m_listCurve.front();
        m_listCurve.pop_front();
    }
    while (!m_listPlot.empty())
    {
        delete m_listPlot.front();
        m_listPlot.pop_front();
    }

    for (int i(0) ; i < m_listNbrOfCurves.size() ; i++)
    {
        QwtPlot* pPlot = new QwtPlot();
        pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
        pPlot->setAxisTitle(QwtPlot::xBottom, "Sec");

        m_listPlot.push_back(pPlot);
        m_GraphsLayout.addWidget(pPlot);
    }

    for (int i(0), j(0), k(0) ; i < m_pDoc->GetWishBoneRegisterList()->size() ; i++)
    {
        QwtPlotCurve* pCurve = new QwtPlotCurve(m_pDoc->GetWishBoneRegisterList()->value(i)->Name());
        pCurve->setStyle(QwtPlotCurve::Lines);
        pCurve->setCurveAttribute(QwtPlotCurve::Inverted);
        pCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
        pCurve->setPen(QColor(Qt::darkGreen));

        pCurve->attach(m_listPlot[k]);
        m_listPlot[k]->setAxisTitle(QwtPlot::yLeft, m_pDoc->GetWishBoneRegisterList()->value(i)->Unit());
        m_listCurve.push_back(pCurve);

        if (++j == m_listNbrOfCurves[k])
        {
            j = 0;
            k++;
        }
    }

    return true;
}

bool GraphTab::UpdateLayout()
{
    if (!m_listPlot.empty())
    {
        QRect Contents(contentsRect());
        for (int i(0) ; i < m_listPlot.size() ; i++)
        {
            QRect Rect(5,
                       40 + ((Contents.height() - 35)/m_listPlot.size())*i,
                       Contents.width() - 20,
                       Contents.height()/m_listPlot.size() - 40);
            m_listPlot[i]->setGeometry(Rect);
        }
    }

    return true;
}

bool GraphTab::UpdateData()
{
    for (int i(0) ; i < m_listCurve.size() ; i++)
    {
        m_listCurve[i]->setData(m_pDoc->GetWishBoneRegisterList()->value(i)->DateTab(),
                                m_pDoc->GetWishBoneRegisterList()->value(i)->ValueTab());
    }

    for (int i(0) ; i < m_listPlot.size(); i++)
    {
        m_listPlot[i]->replot();
    }

    return true;
}

bool GraphTab::UpdateButtons()
{
    m_StartAllButton.setEnabled(m_pMailBox->IsConnected());
    m_StopAllButton.setEnabled(m_pMailBox->IsConnected());

    return true;
}

void GraphTab::AddCurve()
{
    AddRegisterDialog Dlg(false, this);
    Dlg.setModal(true);
    if (Dlg.exec() == QDialog::Accepted)
    {
        int CurveNbr(0);
        WishBoneRegister* Reg = new WishBoneRegister(Dlg.Name(),
                                                     Dlg.Address(),
                                                     Dlg.ValueMin(),
                                                     Dlg.ValueMax(),
                                                     Dlg.Signed(),
                                                     Dlg.Unit(),
                                                     Dlg.Write_nRead());

        while (m_listNbrOfCurves.size() < (AddCurveToPlot+1))
            m_listNbrOfCurves.push_back(0);
        for (int i(0) ; i < AddCurveToPlot ; i++)
        {
            CurveNbr += m_listNbrOfCurves[i];
        }
        m_pDoc->GetWishBoneRegisterList()->insert(CurveNbr, Reg);
        m_listNbrOfCurves[AddCurveToPlot]++;

        UpdateRegisters();
        UpdateLayout();
    }
}

void GraphTab::AddGraph()
{
    AddCurveToPlot = m_listNbrOfCurves.size();
    AddCurve();
}

void GraphTab::StartAll()
{
    for (int i(0) ; i < m_pDoc->GetWishBoneRegisterList()->size() ; i++)
    {
        m_pDoc->GetWishBoneRegisterList()->value(i)->ResetTab();
        m_pDoc->GetWishBoneRegisterList()->value(i)->SetPeriod(m_PeriodeEdit.text().toUInt());
        m_pMailBox->SendRegister(m_pDoc->GetWishBoneRegisterList()->value(i));
    }
}

void GraphTab::StopAll()
{
    for (int i(0) ; i < m_pDoc->GetWishBoneRegisterList()->size() ; i++)
    {
        m_pDoc->GetWishBoneRegisterList()->value(i)->SetPeriod(0);
        m_pMailBox->SendRegister(m_pDoc->GetWishBoneRegisterList()->value(i));
    }
}

void GraphTab::contextMenuEvent(QContextMenuEvent * event)
{
        if (m_listNbrOfCurves.size() == 0)
        {
            QMenu * menu = new QMenu(this);
            menu->addAction("Ajouter une courbe à un nouveau graphe", this, SLOT(AddGraph()));

            menu->exec(event->globalPos());
        }
        else if(event->x() > 0 && event->x() < width() &&
           event->y() > 0 && event->y() < 40)
        {
            QMenu * menu = new QMenu(this);
            menu->addAction("Ajouter une courbe à un nouveau graphe", this, SLOT(AddGraph()));

            menu->exec(event->globalPos());
        }
        else if(event->x() > 0 && event->x() < width() &&
                event->y() > 40 && event->y() < height())
        {
            AddCurveToPlot = (int)((event->y() - 35)/(double)((height() - 35)/(m_listNbrOfCurves.size())));
            QMenu * menu = new QMenu(this);
            menu->addAction("Ajouter une courbe à ce graphe", this, SLOT(AddCurve()));
            menu->addAction("Ajouter une courbe à un nouveau graphe", this, SLOT(AddGraph()));

            menu->exec(event->globalPos());
        }
}
