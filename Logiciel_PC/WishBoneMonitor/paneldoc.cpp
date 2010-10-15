#include "paneldoc.h"

PanelDoc::PanelDoc(const QString &Title, bool* pConnectedMode)
    : m_Title(Title)
    , m_pConnectedMode(pConnectedMode)
{
}

PanelDoc::~PanelDoc()
{
}

WishBoneWidgetDoc* PanelDoc::AddWidget()
{
    WishBoneWidgetDoc* Widget = new WishBoneWidgetDoc(m_pConnectedMode);
    m_listWidget.push_back(Widget);

    return Widget;
}
