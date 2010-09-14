#include "controltab.h"
#include "addregisterdialog.h"
#include "wishboneregister.h"
#include <math.h>

ControlTab::ControlTab(MailBoxDriver *pMailBox, PanelDoc *pDoc, QWidget *parent)
    : VirtualTab(parent)
    , m_pMailBox(pMailBox)
    , m_pDoc(pDoc)
{
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
        RegisterDisplay* Reg = new RegisterDisplay(m_pMailBox, m_pDoc->GetWishBoneRegisterList()->value(i));
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
        m_listRegisterDisplay[i]->UpdateReadValue();
        m_listRegisterDisplay[i]->UpdateButtonEnable();
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

void ControlTab::AddRegister()
{
    AddRegisterDialog Dlg(this);
    Dlg.setModal(true);
    if (Dlg.exec() == QDialog::Accepted)
    {
        WishBoneRegister* Reg = new WishBoneRegister(Dlg.Name(),
                                                     Dlg.Address(),
                                                     Dlg.ValueMin(),
                                                     Dlg.ValueMax(),
                                                     Dlg.Unit(),
                                                     Dlg.Write_nRead());
        m_pDoc->AddRegister(Reg);
        UpdateRegisters();
        UpdateLayout();
    }
}
