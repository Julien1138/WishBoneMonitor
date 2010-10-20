#include "paneldoc.h"

PanelDoc::PanelDoc(const QString &Title, MailBoxDriver* pMailBox, QList<WishBoneRegister*>* plistRegisters)
    : m_Title(Title)
    , m_pMailBox(pMailBox)
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
