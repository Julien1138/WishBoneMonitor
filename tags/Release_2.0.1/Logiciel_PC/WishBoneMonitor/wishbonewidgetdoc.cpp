#include "wishbonewidgetdoc.h"

WishBoneWidgetDoc::WishBoneWidgetDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : m_Title(Title)
    , m_pMailBox(pMailBox)
    , m_X(X)
    , m_Y(Y)
    , m_Width(Width)
    , m_Height(Height)
{
}

WishBoneWidgetDoc::~WishBoneWidgetDoc()
{
}

void WishBoneWidgetDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    m_Title =   pSettings->value("Title").toString();
    m_X =       pSettings->value("PosX").toInt();
    m_Y =       pSettings->value("PosY").toInt();
    m_Width =   pSettings->value("Width").toInt();
    m_Height =  pSettings->value("Height").toInt();
}

void WishBoneWidgetDoc::Save(QSettings *pSettings)
{
    pSettings->setValue("Title", m_Title);
    pSettings->setValue("PosX", m_X);
    pSettings->setValue("PosY", m_Y);
    pSettings->setValue("Width", m_Width);
    pSettings->setValue("Height", m_Height);
}

int RegisterIdx(QList<WishBoneRegister*>* plistRegisters, const unsigned long &Address, const bool &Write_nRead)
{
    for (int i(0) ; i < plistRegisters->count() ; i++)
    {
        if (plistRegisters->value(i)->Address() == Address &&
            plistRegisters->value(i)->Write_nRead() == Write_nRead)
        {
            return i;
        }
    }

    return -1;
}
