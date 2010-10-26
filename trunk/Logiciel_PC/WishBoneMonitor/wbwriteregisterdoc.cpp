#include "wbwriteregisterdoc.h"

WBWriteRegisterDoc::WBWriteRegisterDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
    , m_HasSetButton(true)
{
}

WBWriteRegisterDoc::~WBWriteRegisterDoc()
{
}

void WBWriteRegisterDoc::Save(QSettings *pSettings)
{
    WishBoneWidgetDoc::Save(pSettings);

    pSettings->setValue("Widget", "WBWriteRegister");
    pSettings->setValue("HasSetButton", m_HasSetButton);
    pSettings->setValue("RegisterAddress", QString::number(m_pRegister->Address()));
    pSettings->setValue("RegisterWrite_nRead", QString::number(m_pRegister->Write_nRead()));
}

void WBWriteRegisterDoc::WriteRegister(unsigned long Value)
{
    m_pRegister->SetValue(Value);
    m_pMailBox->SendRegister(m_pRegister);
}
