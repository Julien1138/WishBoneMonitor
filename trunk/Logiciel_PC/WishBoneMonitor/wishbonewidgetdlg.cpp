#include "wishbonewidgetdlg.h"

WishBoneWidgetDlg::WishBoneWidgetDlg(QList<WishBoneRegister*>* plistRegisters, WishBoneWidgetDoc* pDoc, QWidget *parent)
    : QDialog(parent)
    , m_plistRegisters(plistRegisters)
    , m_pDoc(pDoc)
    , m_EditTitle(pDoc->Title())
    , m_OKButton("OK")
    , m_CancelButton("Cancel")
{
    connect(&m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(&m_CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void WishBoneWidgetDlg::SetTitle(QString Title)
{
    m_pDoc->SetTitle(Title);
}

void WishBoneWidgetDlg::OnAccept()
{

}

void WishBoneWidgetDlg::UpdateData()
{

}
