#ifndef WISHBONEWIDGETVIEW_H
#define WISHBONEWIDGETVIEW_H

#include <QWidget>
#include <QPoint>
#include <QGroupBox>
#include <QLayout>
#include <QAction>

#include "WishBoneWidgetDoc.h"
#include "WishBoneWidgetDlg.h"

class WishBoneWidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit WishBoneWidgetView(WishBoneWidgetDoc*  pDoc, WishBoneWidgetDlg*  pDlg, QWidget *parent = 0);

signals:

public slots:
    virtual void    ModeChanged() = 0;
    virtual void    Refresh() = 0;
    void    Configure();
    void    Delete();

protected:
    WishBoneWidgetDoc*  m_pDoc;
    WishBoneWidgetDlg*  m_pDlg;

private:
    QPoint  m_MouseRelativePosition;
    bool    m_IsBeingMoved;
    bool    m_IsBeingResized;
    void    mousePressEvent(QMouseEvent *event);
    void    mouseMoveEvent(QMouseEvent *event);
    void    mouseReleaseEvent(QMouseEvent *event);

protected:
    QGroupBox   m_GroupBox;
    QLayout*    m_pLayout;

    bool    eventFilter(QObject *obj, QEvent *event);

    void    UpdateDisplay();
    virtual void    UpdateWidget();

    void    contextMenuEvent(QContextMenuEvent * event);
    QAction*    m_pConfigAction;
    QAction*    m_pDeleteAction;
};

#endif // WISHBONEWIDGETVIEW_H
