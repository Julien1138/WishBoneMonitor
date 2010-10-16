#include "wishbonewidgetview.h"
#include <QMouseEvent>

WishBoneWidgetView::WishBoneWidgetView(WishBoneWidgetDoc*  pDoc, QWidget *parent) :
    QWidget(parent) ,
    m_pDoc(pDoc),
    m_IsBeingMoved(false)
{
    m_pGroupBox = new QGroupBox("GroupBox", this);
    //m_pLabel = new QLabel("Label", m_pGroupBox);
}

void WishBoneWidgetView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MousePreviousPosition = event->pos();
        m_IsBeingMoved = true;
    }
}

void WishBoneWidgetView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && m_IsBeingMoved)
    {
        QPoint PointTemp(event->x(), event->y());
        PointTemp -= m_MousePreviousPosition;
        setGeometry(x() + PointTemp.x()
                  , y() + PointTemp.y()
                  , width()
                  , height());
    }
}

void WishBoneWidgetView::mouseReleaseEvent(QMouseEvent *event)
{
    m_IsBeingMoved = false;
}
