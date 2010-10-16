#ifndef PANELVIEW_H
#define PANELVIEW_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "PanelDoc.h"
#include "WishBoneWidgetView.h"

class PanelView : public QWidget
{
    Q_OBJECT
public:
    explicit PanelView(PanelDoc* pDoc, QWidget *parent = 0);

    PanelDoc*                   pDoc(){return m_pDoc;}
    QList<WishBoneWidgetView*>* GetWidgetList(){return &m_listWidget;}

signals:

public slots:
    void    AddWidget();

private:
    PanelDoc*                   m_pDoc;
    QList<WishBoneWidgetView*>  m_listWidget;


    void contextMenuEvent(QContextMenuEvent * event);
    QPoint  m_ContextMenuPosition;
};

#endif // PANELVIEW_H
