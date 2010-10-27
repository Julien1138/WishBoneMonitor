#include "wbwriteregisterdoc.h"

WBWriteRegisterDoc::WBWriteRegisterDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
{
}

WBWriteRegisterDoc::~WBWriteRegisterDoc()
{
}

void WBWriteRegisterDoc::Save(QSettings *pSettings)
{
    WishBoneWidgetDoc::Save(pSettings);

    pSettings->setValue("Widget", "WBWriteRegister");
    pSettings->setValue("RegisterAddress", QString::number(m_pRegister->Address()));
    pSettings->setValue("RegisterWrite_nRead", m_pRegister->Write_nRead() ? "true" : "false");
}

void WBWriteRegisterDoc::WriteRegister(unsigned long Value)
{
    m_pRegister->SetValue(Value);
    m_pMailBox->SendRegister(m_pRegister);
}
