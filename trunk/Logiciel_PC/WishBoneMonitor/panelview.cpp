#include "panelview.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QStringList>
#include <QInputDialog>

PanelView::PanelView(PanelDoc* pDoc, QWidget *parent) :
    QWidget(parent),
    m_pDoc(pDoc)
{
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
        WishBoneWidgetDlg* WidgetDlg;
        WishBoneWidgetView* WidgetView;
        if (WidgetType == "WriteRegister")
        {
            WidgetDoc = new WBWriteRegisterDoc(m_pDoc->GetplistRegisters()->front()->Name()
                                             , m_pDoc->GetpConnectedMode()
                                             , m_ContextMenuPosition.x()
                                             , m_ContextMenuPosition.y());
            m_pDoc->AddWidget(WidgetDoc);
            WidgetDlg = new WBWriteRegisterDlg(pDoc()->GetplistRegisters()
                                             , (WBWriteRegisterDoc*) WidgetDoc
                                             , this);
            WidgetDlg->setModal(true);
            if (WidgetDlg->exec() == QDialog::Accepted)
            {
                WidgetView = new WBWriteRegisterView((WBWriteRegisterDoc*) WidgetDoc, this);
            }
            else
            {
                delete WidgetDoc;
                return;
            }
            delete WidgetDlg;
        }
        else
        {
            return;
        }

        m_listWidget.push_back(WidgetView);

        WidgetView->show();
    }
}

void PanelView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_pDoc->GetplistRegisters()->empty())
    {
        m_ContextMenuPosition = mapFromGlobal(event->globalPos());

        QMenu * menu = new QMenu(this);
        menu->addAction("Ajouter un Widget", this, SLOT(AddWidget()));

        menu->exec(event->globalPos());
    }
}
