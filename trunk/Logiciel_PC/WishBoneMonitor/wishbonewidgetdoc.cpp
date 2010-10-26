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

void WishBoneWidgetDoc::Save(QSettings *pSettings)
{
    pSettings->setValue("Title", m_Title);
    pSettings->setValue("PosX", m_X);
    pSettings->setValue("PosY", m_Y);
    pSettings->setValue("Width", m_Width);
    pSettings->setValue("Height", m_Height);
}
