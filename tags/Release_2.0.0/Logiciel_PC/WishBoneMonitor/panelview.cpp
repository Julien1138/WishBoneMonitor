#include "panelview.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QStringList>
#include <QInputDialog>

PanelView::PanelView(PanelDoc* pDoc, QWidget *parent) :
    QWidget(parent),
    m_pDoc(pDoc)
{
    m_listWidget.clear();
    m_listWidgetDlg.clear();
}

void PanelView::ModeChanged()
{
    emit ChangeMode();
}

void PanelView::AddWidget()
{
    bool ok;
    QStringList WidgetTypeList;
    WidgetTypeList << "WriteRegister" << "ReadRegister";
    QString WidgetType(QInputDialog::getItem(this, "Ajouter un Widget", "Quel type de Widget\nvoulez vous ajouter ?", WidgetTypeList, 0, false, &ok));

    if(ok)
    {
        WishBoneWidgetDoc* WidgetDoc;
        if (WidgetType == "WriteRegister")
        {
            WidgetDoc = new WBWriteRegisterDoc(WidgetType
                                             , m_pDoc->GetMailBox()
                                             , m_ContextMenuPosition.x()
                                             , m_ContextMenuPosition.y());
        }
        else if (WidgetType == "ReadRegister")
        {
            WidgetDoc = new WBReadRegisterDoc(WidgetType
                                             , m_pDoc->GetMailBox()
                                             , m_ContextMenuPosition.x()
                                             , m_ContextMenuPosition.y());
        }
        else
        {
            return;
        }
        m_pDoc->AddWidget(WidgetDoc);
        RedrawAllWidgets();

        m_listWidgetDlg.back()->setModal(true);
        if (m_listWidgetDlg.back()->exec() != QDialog::Accepted)
        {
            m_pDoc->DeleteWidget(WidgetDoc);
        }
        RedrawAllWidgets();
    }
}

void PanelView::DeleteWidget(WishBoneWidgetDoc *Widget)
{
    m_pDoc->DeleteWidget(Widget);
    RedrawAllWidgets();
}

void PanelView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_pDoc->GetRegistersList()->empty())
    {
        m_ContextMenuPosition = mapFromGlobal(event->globalPos());

        QMenu * menu = new QMenu(this);
        menu->addAction("Ajouter un Widget", this, SLOT(AddWidget()));

        menu->exec(event->globalPos());
    }
}

void PanelView::RedrawAllWidgets()
{
    while (!(m_listWidget.isEmpty()))
    {
       /* delete (m_listWidget.front());*/
        m_listWidget.front()->deleteLater();
        m_listWidget.pop_front();
    }
    while (!(m_listWidgetDlg.isEmpty()))
    {
        delete (m_listWidgetDlg.front());
        m_listWidgetDlg.pop_front();
    }

    for (int i(0) ; i < m_pDoc->GetWidgetList()->count() ; i++)
    {
        WishBoneWidgetView* WidgetView;
        WishBoneWidgetDlg* WidgetDlg;
        if (m_pDoc->GetWidgetList()->value(i)->GetType() == eWriteRegister)
        {
            WidgetDlg = new WBWriteRegisterDlg(pDoc()->GetRegistersList()
                                             , (WBWriteRegisterDoc*) m_pDoc->GetWidgetList()->value(i)
                                             , this);
            WidgetView = new WBWriteRegisterView((WBWriteRegisterDoc*) m_pDoc->GetWidgetList()->value(i)
                                               , (WBWriteRegisterDlg*) WidgetDlg
                                               , this);
        }
        else if (m_pDoc->GetWidgetList()->value(i)->GetType() == eReadRegister)
        {
            WidgetDlg = new WBReadRegisterDlg(pDoc()->GetRegistersList()
                                            , (WBReadRegisterDoc*) m_pDoc->GetWidgetList()->value(i)
                                            , this);
            WidgetView = new WBReadRegisterView((WBReadRegisterDoc*) m_pDoc->GetWidgetList()->value(i)
                                              , (WBReadRegisterDlg*) WidgetDlg
                                              , this);
        }
        else
        {
            continue;
        }
        connect(this, SIGNAL(ChangeMode()), WidgetView, SLOT(ModeChanged()));
        m_listWidget.append(WidgetView);
        m_listWidgetDlg.append(WidgetDlg);
        WidgetView->show();
    }
}
