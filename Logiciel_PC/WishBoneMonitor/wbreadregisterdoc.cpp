#include "wbreadregisterdoc.h"

WBReadRegisterDoc::WBReadRegisterDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WBREAD_WIDTH_MIN
                      , 80)
    , m_pRegister(NULL)
{
}

WBReadRegisterDoc::WBReadRegisterDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
    , m_pRegister(NULL)
{
}

WBReadRegisterDoc::~WBReadRegisterDoc()
{
}

void WBReadRegisterDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    WishBoneWidgetDoc::Load(pSettings, plistRegisters);

    int Idx(RegisterIdx(plistRegisters
          , pSettings->value("RegisterAddress").toUInt()
          , pSettings->value("RegisterWrite_nRead").toString() == "true" ? true : false));

    m_pRegister = plistRegisters->at(Idx);
}

void WBReadRegisterDoc::Save(QSettings *pSettings)
{
    WishBoneWidgetDoc::Save(pSettings);

    pSettings->setValue("Widget", "WBReadRegister");
    pSettings->setValue("RegisterAddress", QString::number(m_pRegister->Address()));
    pSettings->setValue("RegisterWrite_nRead", m_pRegister->Write_nRead() ? "true" : "false");
}

void WBReadRegisterDoc::ReadRegister()
{
    m_pMailBox->SendRegister(m_pRegister);
}
