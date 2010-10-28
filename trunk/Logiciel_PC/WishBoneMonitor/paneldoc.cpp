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

void PanelDoc::Save(QSettings *pSettings)
{
    pSettings->setValue("Title", m_Title);
    pSettings->setValue("NbrOfWidgets", QString::number(m_listWidget.count()));
    for (int i(0) ; i < m_listWidget.count() ; i++)
    {
        pSettings->beginGroup("Widget_" + QString::number(i));
        m_listWidget.value(i)->Save(pSettings);
        pSettings->endGroup();
    }
}

void PanelDoc::AddWidget(WishBoneWidgetDoc* Widget)
{
    m_listWidget.push_back(Widget);
}

void PanelDoc::DeleteWidget(WishBoneWidgetDoc *Widget)
{
    m_listWidget.removeOne(Widget);
    delete Widget;
}
