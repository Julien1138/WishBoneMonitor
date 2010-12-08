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
#include "WBReadRegisterDoc.h"
#include "WBReadRegisterView.h"
#include "WBReadRegisterDlg.h"
#include "WBGraphDoc.h"
#include "WBGraphView.h"
#include "WBGraphDlg.h"

class PanelView : public QWidget
{
    Q_OBJECT
public:
    explicit PanelView(PanelDoc* pDoc, QWidget *parent = 0);

    void RedrawAllWidgets();

    PanelDoc*                   pDoc(){return m_pDoc;}
    QList<WishBoneWidgetView*>* GetWidgetList(){return &m_listWidget;}

signals:
    void    ChangeMode();

public slots:
    void    ModeChanged();
    void    AddWidget();
    void    DeleteWidget(WishBoneWidgetDoc* Widget);

private:
    PanelDoc*                   m_pDoc;
    QList<WishBoneWidgetView*>  m_listWidget;
    QList<WishBoneWidgetDlg*>   m_listWidgetDlg;

    void contextMenuEvent(QContextMenuEvent * event);
    QPoint  m_ContextMenuPosition;
};

#endif // PANELVIEW_H
