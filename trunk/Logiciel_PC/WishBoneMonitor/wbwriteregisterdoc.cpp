#include "wbwriteregisterdoc.h"

WBWriteRegisterDoc::WBWriteRegisterDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WIDTH_MIN
                      , HEIGHT_MIN)
{
}

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

void WBWriteRegisterDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    m_pRegister = plistRegisters->value(RegisterIdx(plistRegisters
                                                  , pSettings->value("RegisterAddress").toUInt()
                                                  , pSettings->value("RegisterWrite_nRead").toString() == "true" ? true : false));

    WishBoneWidgetDoc::Load(pSettings, plistRegisters);
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
