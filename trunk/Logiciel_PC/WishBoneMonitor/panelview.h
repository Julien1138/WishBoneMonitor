#ifndef PANELVIEW_H
#define PANELVIEW_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "PanelDoc.h"
#include "WishBoneWidgetDoc.h"
#include "WishBoneWidgetView.h"
#include "WishBoneWidgetDlg.h"
#include "WBWriteRegisterDoc.h"
#include "WBWriteRegisterView.h"
#include "WBWriteRegisterDlg.h"

class PanelView : public QWidget
{
    Q_OBJECT
public:
    explicit PanelView(PanelDoc* pDoc, QWidget *parent = 0);

    PanelDoc*                   pDoc(){return m_pDoc;}
    QList<WishBoneWidgetView*>* GetWidgetList(){return &m_listWidget;}

signals:
    void    ChangeMode();

public slots:
    void    ModeChanged();
    void    AddWidget();
    void    DeleteWidget(WishBoneWidgetView* Widget);

private:
    PanelDoc*                   m_pDoc;
    QList<WishBoneWidgetView*>  m_listWidget;


    void contextMenuEvent(QContextMenuEvent * event);
    QPoint  m_ContextMenuPosition;
};

#endif // PANELVIEW_H
