#include "graphtab.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QRect>

GraphTab::GraphTab(QWidget *parent)
    : VirtualTab(parent)
{
    m_listGraphDisplay.clear();
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
    for (int i(0) ; i < m_listGraphDisplay.size() ; i++)
    {
        m_Layout.addWidget(m_listGraphDisplay[i]);
    }
    setLayout(&m_Layout);
    if (m_listGraphDisplay.size())
    {
        QRect Rect(contentsRect());
        Rect.setWidth(Rect.width() - 20);
        Rect.setHeight(Rect.height()/m_listGraphDisplay.size() - 15);
        for (int i(0) ; i < m_listGraphDisplay.size() ; i++)
        {
            m_listGraphDisplay[i]->UpdateDisplay(Rect);
        }
    }

    return true;
}

void GraphTab::AddGraph()
{
    GraphDisplay* Graph = new GraphDisplay;
    connect(Graph, SIGNAL(Delete(GraphDisplay*)), this, SLOT(DelGraph(GraphDisplay*)));
    m_listGraphDisplay.push_back(Graph);

    UpdateLayout();
}

void GraphTab::DelGraph(GraphDisplay* Graph)
{
    m_listGraphDisplay.removeOne(Graph);
    delete Graph;

    UpdateLayout();
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
