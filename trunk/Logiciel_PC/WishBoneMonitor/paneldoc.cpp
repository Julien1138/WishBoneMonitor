#include "paneldoc.h"

PanelDoc::PanelDoc(const QString &Title, bool* pConnectedMode, QList<WishBoneRegister*>* plistRegisters)
    : m_Title(Title)
    , m_pConnectedMode(pConnectedMode)
    , m_plistRegisters(plistRegisters)
{
}

PanelDoc::~PanelDoc()
{
}

void PanelDoc::AddWidget(WishBoneWidgetDoc* Widget)
{
    m_listWidget.push_back(Widget);
}
