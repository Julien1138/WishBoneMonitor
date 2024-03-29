#include "wbreadregisterdlg.h"
#include <QFormLayout>

WBReadRegisterDlg::WBReadRegisterDlg(QList<WishBoneRegister*>* plistRegisters, WBReadRegisterDoc* pDoc, QWidget *parent)
    : WishBoneWidgetDlg(plistRegisters, pDoc, parent)
    , m_ComboRegisterChoice(this)
    , m_MainLayout()
    , m_ButtonsLayout()
{
    m_pLayout = new QFormLayout();

    for (int i(0) ; i < m_plistRegisters->count() ; i++)
    {
        m_ComboRegisterChoice.addItem(m_plistRegisters->value(i)->Name());
    }
    ((QFormLayout*) m_pLayout)->addRow("Nom du registre", &m_ComboRegisterChoice);

    ((QFormLayout*) m_pLayout)->addRow("Titre du Widget", &m_EditTitle);

    m_MainLayout.addLayout(m_pLayout);

    connect(&m_OKButton, SIGNAL(clicked()), this, SLOT(OnAccept()));
    m_ButtonsLayout.addWidget(&m_OKButton);
    m_ButtonsLayout.addWidget(&m_CancelButton);
    m_MainLayout.addLayout(&m_ButtonsLayout);

    setLayout(&m_MainLayout);
}

void WBReadRegisterDlg::OnAccept()
{
    WishBoneWidgetDlg::OnAccept();

    ((WBReadRegisterDoc*) m_pDoc)->SetpRegister(m_plistRegisters->value(m_ComboRegisterChoice.currentIndex()));
}

void WBReadRegisterDlg::UpdateData()
{
    WishBoneWidgetDlg::UpdateData();

    m_ComboRegisterChoice.clear();
    for (int i(0) ; i < m_plistRegisters->count() ; i++)
    {
        m_ComboRegisterChoice.addItem(m_plistRegisters->value(i)->Name());
    }

    int CurrentRegisterIdx(m_plistRegisters->indexOf(((WBReadRegisterDoc*) m_pDoc)->Register()));
    if (CurrentRegisterIdx != -1)
    {
        m_ComboRegisterChoice.setCurrentIndex(CurrentRegisterIdx);
    }
}
