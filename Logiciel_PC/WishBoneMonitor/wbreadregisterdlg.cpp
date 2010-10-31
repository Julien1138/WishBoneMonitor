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
    ((WBReadRegisterDoc*) m_pDoc)->SetpRegister(m_plistRegisters->front());
    connect(&m_ComboRegisterChoice, SIGNAL(currentIndexChanged(QString)), &m_EditTitle, SLOT(setText(QString)));
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
    ((WBReadRegisterDoc*) m_pDoc)->SetTitle(m_EditTitle.text());
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
    m_ComboRegisterChoice.setCurrentIndex(m_plistRegisters->indexOf(((WBReadRegisterDoc*) m_pDoc)->Register()));
}
