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
