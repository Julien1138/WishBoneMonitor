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

void WBWriteRegisterDoc::WriteRegister(unsigned long Value)
{
    m_pRegister->SetValue(Value);
    m_pMailBox->SendRegister(m_pRegister);
}
