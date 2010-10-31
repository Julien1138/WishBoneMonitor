#include "wishbonemonitor.h"

WishBoneMonitor::WishBoneMonitor()
{
    m_pMailBox = new MailBoxDriver();
}

WishBoneMonitor::~WishBoneMonitor()
{
    ClearPanelList();
    ClearRegisterList();
    delete m_pMailBox;
}

void WishBoneMonitor::Load(QSettings *pSettings)
{
    int size;

    pSettings->beginGroup("RegisterList");
    size = pSettings->beginReadArray("Registers");
    for (int i(0) ; i < size ; i++)
    {
        pSettings->setArrayIndex(i);
        WishBoneRegister* Reg = new WishBoneRegister();
        Reg->Load(pSettings);
        m_listRegisters.append(Reg);
    }
    pSettings->endArray();
    pSettings->endGroup();

    pSettings->beginGroup("PanelList");
    size = pSettings->beginReadArray("Panels");
    for (int i(0) ; i < size ; i++)
    {
        pSettings->setArrayIndex(i);
        PanelDoc* Panel = new PanelDoc(m_pMailBox, &m_listRegisters);
        Panel->Load(pSettings);
        m_listPanel.append(Panel);
    }
    pSettings->endArray();
    pSettings->endGroup();
}

void WishBoneMonitor::Save(QSettings *pSettings)
{
    pSettings->beginGroup("RegisterList");
    pSettings->beginWriteArray("Registers");
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        pSettings->setArrayIndex(i);
        m_listRegisters.value(i)->Save(pSettings);
    }
    pSettings->endArray();
    pSettings->endGroup();

    pSettings->beginGroup("PanelList");
    pSettings->beginWriteArray("Panels");
    for (int i(0) ; i < m_listPanel.count() ; i++)
    {
        pSettings->setArrayIndex(i);
        m_listPanel.value(i)->Save(pSettings);
    }
    pSettings->endArray();
    pSettings->endGroup();
}

PanelDoc* WishBoneMonitor::AddPanel(const QString &PanelTitle)
{
    PanelDoc* Panel = new PanelDoc(PanelTitle, m_pMailBox, &m_listRegisters);
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

bool WishBoneMonitor::DelRegister(const unsigned long &Address, const bool &Write_nRead)
{
    int Idx = RegisterIdx(Address, Write_nRead);

    if (Idx == -1)
        return false;

    delete m_listRegisters.value(Idx);
    m_listRegisters.removeAt(Idx);

    return true;
}

void WishBoneMonitor::DelRegister(const int Idx)
{
    delete m_listRegisters.value(Idx);
    m_listRegisters.removeAt(Idx);
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

int WishBoneMonitor::RegisterIdx(const unsigned long &Address, const bool &Write_nRead) const
{
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        if (m_listRegisters.value(i)->Address() == Address &&
            m_listRegisters.value(i)->Write_nRead() == Write_nRead)
        {
            return i;
        }
    }

    return -1;
}

void WishBoneMonitor::SetupMailBox()
{
    for (int i(0) ; i < m_listRegisters.count() ; i++)
    {
        if (m_listRegisters.value(i)->Period())
        {
            m_pMailBox->SendRegister(m_listRegisters.value(i));
        }
    }
}

void WishBoneMonitor::UnsetupMailBox()
{

}
