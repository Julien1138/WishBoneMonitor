#ifndef WISHBONEWIDGETVIEW_H
#define WISHBONEWIDGETVIEW_H

#include <QWidget>
#include <QPoint>
#include <QGroupBox>
#include <QLayout>

#include "WishBoneWidgetDoc.h"

class WishBoneWidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit WishBoneWidgetView(WishBoneWidgetDoc*  pDoc, QWidget *parent = 0);

signals:

public slots:
    virtual void    ModeChanged() = 0;

protected:
    WishBoneWidgetDoc*  m_pDoc;

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
};

#endif // WISHBONEWIDGETVIEW_H
