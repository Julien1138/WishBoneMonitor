#include "PanelDoc.h"
#include <QString>
#include "WBWriteRegisterDoc.h"
#include "WBReadRegisterDoc.h"
#include "WBGraphDoc.h"

PanelDoc::PanelDoc(MailBoxDriver* pMailBox, QList<WishBoneRegister*>* plistRegisters)
    : m_Title("")
    , m_pMailBox(pMailBox)
    , m_plistRegisters(plistRegisters)
{
}

PanelDoc::PanelDoc(const QString &Title, MailBoxDriver* pMailBox, QList<WishBoneRegister*>* plistRegisters)
    : m_Title(Title)
    , m_pMailBox(pMailBox)
    , m_plistRegisters(plistRegisters)
{
}

PanelDoc::~PanelDoc()
{

}

void PanelDoc::Load(QSettings *pSettings)
{
    m_Title = pSettings->value("Title").toString();
    int size = pSettings->beginReadArray("Widgets");
    for (int i(0) ; i < size ; i++)
    {
        pSettings->setArrayIndex(i);
        WishBoneWidgetDoc* Widget;
        QString WidgetType(pSettings->value("Widget").toString());
        if (WidgetType == "WBWriteRegister")
        {
            Widget = new WBWriteRegisterDoc(m_pMailBox);
        }
        else if (WidgetType == "WBReadRegister")
        {
            Widget = new WBReadRegisterDoc(m_pMailBox);
        }
        else if (WidgetType == "WBGraph")
        {
            Widget = new WBGraphDoc(m_pMailBox);
        }
        else
        {
            continue;
        }
        Widget->Load(pSettings, m_plistRegisters);
        m_listWidget.push_back(Widget);
    }
    pSettings->endArray();
}

void PanelDoc::Save(QSettings *pSettings)
{
    pSettings->setValue("Title", m_Title);
    pSettings->beginWriteArray("Widgets");
    for (int i(0) ; i < m_listWidget.count() ; i++)
    {
        pSettings->setArrayIndex(i);
        m_listWidget.value(i)->Save(pSettings);
    }
    pSettings->endArray();
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
