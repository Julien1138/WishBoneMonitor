#include "graphtab.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QRect>

GraphTab::GraphTab(MailBoxDriver *pMailBox, PanelDoc *pDoc, QWidget *parent)
    : VirtualTab(parent)
    , m_pMailBox(pMailBox)
    , m_pDoc(pDoc)
{
    m_StartAllButton.setText("Start All");
    m_StartAllButton.setFixedSize(60, 20);
    connect(&m_StartAllButton, SIGNAL(clicked()), this, SLOT(StartAll()));
    m_TopLayout.addWidget(&m_StartAllButton);
    m_StopAllButton.setText("Stop All");
    m_StopAllButton.setFixedSize(60, 20);
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
    while (!m_listGraphDisplay.empty())
    {
        delete m_listGraphDisplay.front();
        m_listGraphDisplay.pop_front();
    }
}

bool GraphTab::UpdateLayout()
{
    if (!m_listGraphDisplay.empty())
    {
        QRect Rect(contentsRect());
        Rect.setWidth(Rect.width() - 20);
        Rect.setHeight(Rect.height()/m_listGraphDisplay.size() - 35);
        for (int i(0) ; i < m_listGraphDisplay.size() ; i++)
        {
            m_listGraphDisplay[i]->UpdateDisplay(Rect);
        }
    }

    return true;
}

bool GraphTab::UpdateData()
{
    for (int i(0) ; i < m_listGraphDisplay.size() ; i++)
    {
        for (int j(0) ; j < m_listGraphDisplay.value(i)->GetpRegisterList()->size() ; j++)
        {
            m_listGraphDisplay.value(i)->GetpCurveList()->value(j)->setData(m_listGraphDisplay.value(i)->GetpRegisterList()->value(j)->DateTab(),
                                                                            m_listGraphDisplay.value(i)->GetpRegisterList()->value(j)->ValueTab());
        }
        m_listGraphDisplay[i]->GetPlot()->replot();
    }

    return true;
}

void GraphTab::AddGraph()
{
    GraphDisplay* Graph = new GraphDisplay(m_pDoc);
    connect(Graph, SIGNAL(Delete(GraphDisplay*)), this, SLOT(DelGraph(GraphDisplay*)));
//    connect(Graph, SIGNAL(AddNew()), this, SLOT(AddGraph()));
    m_listGraphDisplay.push_back(Graph);

    m_GraphsLayout.addWidget(Graph);

    UpdateLayout();
}

void GraphTab::DelGraph(GraphDisplay* Graph)
{
    m_GraphsLayout.removeWidget(Graph);

    delete Graph;
    m_listGraphDisplay.removeOne(Graph);

    UpdateLayout();
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
    if(event->x() > 0 && event->x() < width() &&
       event->y() > 0 && event->y() < height())
    {
        QMenu * menu = new QMenu(this);
        menu->addAction("Ajouter Graph", this, SLOT(AddGraph()));

        menu->exec(event->globalPos());
    }
}
