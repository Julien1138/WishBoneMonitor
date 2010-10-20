#include "wbwriteregisterdlg.h"
#include <QFormLayout>

WBWriteRegisterDlg::WBWriteRegisterDlg(QList<WishBoneRegister*>* plistRegisters, WBWriteRegisterDoc* pDoc, QWidget *parent)
    : WishBoneWidgetDlg(plistRegisters, pDoc, parent)
    , m_ComboRegisterChoice(this)
    , m_MainLayout()
    , m_ButtonsLayout()
{
    m_pLayout = new QFormLayout();

    for (int i(0) ; i < plistRegisters->count() ; i++)
    {
        m_ComboRegisterChoice.addItem(plistRegisters->value(i)->Name());
    }
    connect(&m_ComboRegisterChoice, SIGNAL(currentIndexChanged(int)), this, SLOT(SetpDoc(int)));
    SetpDoc(0);
    connect(&m_ComboRegisterChoice, SIGNAL(currentIndexChanged(QString)), &m_EditTitle, SLOT(setText(QString)));
    ((QFormLayout*) m_pLayout)->addRow("Nom du registre", &m_ComboRegisterChoice);

    connect(&m_EditTitle, SIGNAL(textChanged(QString)), this, SLOT(SetTitle(QString)));
    ((QFormLayout*) m_pLayout)->addRow("Titre du Widget", &m_EditTitle);

    m_MainLayout.addLayout(m_pLayout);

    m_ButtonsLayout.addWidget(&m_OKButton);
    m_ButtonsLayout.addWidget(&m_CancelButton);
    m_MainLayout.addLayout(&m_ButtonsLayout);

    setLayout(&m_MainLayout);
}

void WBWriteRegisterDlg::SetpDoc(int Idx)
{
    ((WBWriteRegisterDoc*) m_pDoc)->SetpRegister(m_plistRegisters->value(Idx));
}
