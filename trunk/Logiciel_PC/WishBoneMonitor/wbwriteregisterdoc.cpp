#include "wbwriteregisterdoc.h"

WBWriteRegisterDoc::WBWriteRegisterDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WBWRITE_WIDTH_MIN
                      , WBWRITE_HEIGHT_MIN)
    , m_pRegister(NULL)
{
}

WBWriteRegisterDoc::WBWriteRegisterDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
    , m_pRegister(NULL)
{
}

WBWriteRegisterDoc::~WBWriteRegisterDoc()
{
}

void WBWriteRegisterDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    WishBoneWidgetDoc::Load(pSettings, plistRegisters);

    int Idx(RegisterIdx(plistRegisters
          , pSettings->value("RegisterAddress").toUInt()
          , pSettings->value("RegisterWrite_nRead").toString() == "true" ? true : false));

    m_pRegister = plistRegisters->at(Idx);
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
