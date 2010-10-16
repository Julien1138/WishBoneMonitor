#include "panelview.h"
#include <QMenu>
#include <QContextMenuEvent>

PanelView::PanelView(PanelDoc* pDoc, QWidget *parent) :
    QWidget(parent),
    m_pDoc(pDoc)
{
}

void PanelView::AddWidget()
{
    WishBoneWidgetView* Widget = new WishBoneWidgetView(pDoc()->AddWidget(), this);
    m_listWidget.push_back(Widget);

    Widget->move(m_ContextMenuPosition);
    Widget->show();
}

void PanelView::contextMenuEvent(QContextMenuEvent *event)
{
    m_ContextMenuPosition = mapFromGlobal(event->globalPos());

    QMenu * menu = new QMenu(this);
    menu->addAction("Ajouter un Widget", this, SLOT(AddWidget()));

    menu->exec(event->globalPos());
}
