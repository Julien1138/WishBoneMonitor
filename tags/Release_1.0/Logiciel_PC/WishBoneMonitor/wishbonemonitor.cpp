#include "wishbonemonitor.h"

WishBoneMonitor::WishBoneMonitor()
{
    m_pMailBox = new MailBoxDriver();
}

WishBoneMonitor::~WishBoneMonitor()
{
    ClearList();
    delete m_pMailBox;
}

void WishBoneMonitor::AddPanel(PanelDoc* Panel)
{
    m_listPanel.append(Panel);
}

void WishBoneMonitor::ClearList()
{
    while (!m_listPanel.empty())
    {
        delete m_listPanel.front();
        m_listPanel.pop_front();
    }
}
