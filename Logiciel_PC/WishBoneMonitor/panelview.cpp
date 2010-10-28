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

void PanelView::ModeChanged()
{
    emit ChangeMode();
}

void PanelView::AddWidget()
{
    bool ok;
    QStringList WidgetTypeList;
    WidgetTypeList << "WriteRegister";/* << "ReadRegister";*/
    QString WidgetType(QInputDialog::getItem(this, "Ajouter un Widget", "Quel type de Widget\nvoulez vous ajouter ?", WidgetTypeList, 0, false, &ok));

    if(ok)
    {
        WishBoneWidgetDoc* WidgetDoc;
        WishBoneWidgetDlg* WidgetDlg;
        WishBoneWidgetView* WidgetView;
        if (WidgetType == "WriteRegister")
        {
            WidgetDoc = new WBWriteRegisterDoc(m_pDoc->GetRegistersList()->front()->Name()
                                             , m_pDoc->GetMailBox()
                                             , m_ContextMenuPosition.x()
                                             , m_ContextMenuPosition.y());
            m_pDoc->AddWidget(WidgetDoc);
            WidgetDlg = new WBWriteRegisterDlg(pDoc()->GetRegistersList()
                                             , (WBWriteRegisterDoc*) WidgetDoc
                                             , this);
            WidgetDlg->setModal(true);
            if (WidgetDlg->exec() == QDialog::Accepted)
            {
                WidgetView = new WBWriteRegisterView((WBWriteRegisterDoc*) WidgetDoc
                                                    ,(WBWriteRegisterDlg*) WidgetDlg
                                                    , this);
                connect(this, SIGNAL(ChangeMode()), WidgetView, SLOT(ModeChanged()));
            }
            else
            {
                delete WidgetDoc;
                delete WidgetDlg;
                return;
            }
        }
        else
        {
            return;
        }

        m_listWidget.push_back(WidgetView);

        WidgetView->show();
    }
}

void PanelView::DeleteWidget(WishBoneWidgetView *Widget)
{
    m_pDoc->DeleteWidget(Widget->WidgetDoc());
    delete Widget->WidgetDlg();
    delete Widget;
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
