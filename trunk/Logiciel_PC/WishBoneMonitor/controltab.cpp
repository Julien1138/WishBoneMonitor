#include "controltab.h"
#include <math.h>

ControlTab::ControlTab(WishBoneMonitor *pDoc, QWidget *parent)
    : QWidget(parent)
    , m_pDoc(pDoc)
{
    UpdateRegisters();
    UpdateLayout();
}

ControlTab::~ControlTab()
{
    while (!m_listRegisterDisplay.empty())
    {
        delete m_listRegisterDisplay.front();
        m_listRegisterDisplay.pop_front();
    }
}

bool ControlTab::UpdateRegisters()
{
    while (!m_listRegisterDisplay.empty())
    {
        delete m_listRegisterDisplay.front();
        m_listRegisterDisplay.pop_front();
    }
    m_listRegisterDisplay.clear();
    for (int i(0) ; i < m_pDoc->GetWishBoneRegisterList()->size() ; i++)
    {
        RegisterDisplay* Reg = new RegisterDisplay(m_pDoc, m_pDoc->GetWishBoneRegisterList()->value(i));
        m_listRegisterDisplay.push_back(Reg);
    }
    return true;
}

bool ControlTab::UpdateLayout()
{
    int MaxNbrOfLines((int)(height()/120));

    for (int i(0) ; i < m_listRegisterDisplay.size() ; i++)
    {
        m_Layout.addWidget(m_listRegisterDisplay[i], i % MaxNbrOfLines, floor(i / MaxNbrOfLines));
    }
    setLayout(&m_Layout);
    return true;
}

bool ControlTab::UpdateData()
{
    for (int i(0) ; i < m_listRegisterDisplay.size() ; i++)
    {
        m_listRegisterDisplay[i]->UpdateReadValue();
    }
    return true;
}

bool ControlTab::UpdateButtons()
{
    for (int i(0) ; i < m_listRegisterDisplay.size() ; i++)
    {
        m_listRegisterDisplay[i]->UpdateButtonEnable();
    }
    return true;
}
