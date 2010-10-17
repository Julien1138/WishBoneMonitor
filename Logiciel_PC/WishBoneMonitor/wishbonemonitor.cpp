#include "wishbonemonitor.h"

WishBoneMonitor::WishBoneMonitor()
    : m_ConnectedMode(false)
{
    m_pMailBox = new MailBoxDriver();
}

WishBoneMonitor::~WishBoneMonitor()
{
    ClearPanelList();
    ClearRegisterList();
    delete m_pMailBox;
}

PanelDoc* WishBoneMonitor::AddPanel(const QString &PanelTitle)
{
    PanelDoc* Panel = new PanelDoc(PanelTitle, &m_ConnectedMode, &m_listRegisters);
    m_listPanel.append(Panel);

    return Panel;
}

void WishBoneMonitor::DelPanel(PanelDoc* Panel)
{
    delete m_listPanel.value(m_listPanel.indexOf(Panel));
    m_listPanel.removeOne(Panel);
}

void WishBoneMonitor::ClearPanelList()
{
    while (!m_listPanel.empty())
    {
        delete m_listPanel.front();
        m_listPanel.pop_front();
    }
}

bool WishBoneMonitor::AddRegister(const WishBoneRegister &Register)
{
    if (RegisterExists(Register))
    {
        return false;
    }

    WishBoneRegister* Reg = new WishBoneRegister(Register);
    m_listRegisters.append(Reg);

    return true;
}

bool WishBoneMonitor::DelRegister(unsigned long Address)
{
    int Idx = RegisterIdx(Address);

    if (Idx == -1)
        return false;

    m_listRegisters.removeAt(Idx);

    return true;
}

void WishBoneMonitor::ClearRegisterList()
{
    while (!m_listRegisters.empty())
    {
        delete m_listRegisters.front();
        m_listRegisters.pop_front();
    }
}

bool WishBoneMonitor::RegisterExists(const WishBoneRegister &Register) const
{
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        if (*(m_listRegisters.value(i)) == Register)
        {
            return true;
        }
    }

    return false;
}

bool WishBoneMonitor::RegisterExists(const unsigned long &Address, const bool &Write_nRead) const
{
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        if (m_listRegisters.value(i)->Address() == Address &&
            m_listRegisters.value(i)->Write_nRead() == Write_nRead)
        {
            return true;
        }
    }

    return false;
}

int WishBoneMonitor::RegisterIdx(unsigned long Addr) const
{
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        if (m_listRegisters.value(i)->Address() == Addr)
        {
            return i;
        }
    }

    return -1;
}
