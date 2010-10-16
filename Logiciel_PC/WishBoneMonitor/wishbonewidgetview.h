#ifndef WISHBONEWIDGETVIEW_H
#define WISHBONEWIDGETVIEW_H

#include <QWidget>
#include <QPoint>
#include <QGroupBox>
#include <QLayout>
#include "wishbonewidgetdoc.h"

#include <QLabel>

class WishBoneWidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit WishBoneWidgetView(WishBoneWidgetDoc*  pDoc, QWidget *parent = 0);

signals:

public slots:

private:
    WishBoneWidgetDoc*  m_pDoc;

    QGroupBox*  m_pGroupBox;
    QLabel*     m_pLabel;

    QPoint  m_MousePreviousPosition;
    bool    m_IsBeingMoved;
    void    mousePressEvent(QMouseEvent *event);
    void    mouseMoveEvent(QMouseEvent *event);
    void    mouseReleaseEvent(QMouseEvent *event);
};

#endif // WISHBONEWIDGETVIEW_H
