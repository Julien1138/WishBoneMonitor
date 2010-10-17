#include "wishbonewidgetview.h"
#include <QMouseEvent>

WishBoneWidgetView::WishBoneWidgetView(WishBoneWidgetDoc*  pDoc, QWidget *parent) :
    QWidget(parent) ,
    m_pDoc(pDoc),
    m_IsBeingMoved(false),
    m_IsBeingResized(false),
    m_GroupBox(this)
{
    m_GroupBox.setTitle(m_pDoc->Title());
    m_GroupBox.installEventFilter(this);
    m_GroupBox.setMouseTracking(true);

    setMouseTracking(true);
}

void WishBoneWidgetView::UpdateDisplay()
{
    setGeometry(m_pDoc->X()
              , m_pDoc->Y()
              , m_pDoc->Width()
              , m_pDoc->Height());
    m_GroupBox.setGeometry(rect());
}

void WishBoneWidgetView::mousePressEvent(QMouseEvent *event)
{
    if (m_pDoc->ConfigMode())
    {
        if(event->button() == Qt::LeftButton)
        {
            if (!m_IsBeingResized)
            {
                m_MouseRelativePosition = QPoint(mapToParent(event->pos()).x() - x()
                                               , mapToParent(event->pos()).y() - y());
            }

            if((mapToParent(event->pos()).x() > (x() + width() - 15)) &&
               (mapToParent(event->pos()).x() < (x() + width())) &&
               (mapToParent(event->pos()).y() > (y() + height() - 15)) &&
               (mapToParent(event->pos()).y() < (y() + height())))
            {
                m_IsBeingResized = true;
            }
            else
            {
                m_IsBeingMoved = true;
            }
        }
    }
}

void WishBoneWidgetView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pDoc->ConfigMode())
    {
        // Changement de l'aspect du curseur
        if (m_IsBeingResized)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if((mapToParent(event->pos()).x() > (x() + width() - 15)) &&
                (mapToParent(event->pos()).x() < (x() + width())) &&
                (mapToParent(event->pos()).y() > (y() + height() - 15)) &&
                (mapToParent(event->pos()).y() < (y() + height())))
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else
        {
            setCursor(Qt::SizeAllCursor);
        }

        // Gestion du deplacement du Widget
        if((event->buttons() & Qt::LeftButton) && m_IsBeingMoved)
        {
            m_pDoc->SetX(mapToParent(event->pos()).x() - m_MouseRelativePosition.x());
            m_pDoc->SetY(mapToParent(event->pos()).y() - m_MouseRelativePosition.y());
            UpdateDisplay();
        }

        // Gestion du redimensionnement du Widget
        if((event->buttons() & Qt::LeftButton) && m_IsBeingResized)
        {
            m_pDoc->SetWidth(mapToParent(event->pos()).x() - x());
            m_pDoc->SetHeight(mapToParent(event->pos()).y() - y());
            UpdateDisplay();
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

void WishBoneWidgetView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pDoc->ConfigMode())
    {
        m_IsBeingResized = false;
        m_IsBeingMoved = false;
    }
}

bool WishBoneWidgetView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        mousePressEvent(static_cast<QMouseEvent *>(event));
    }
    else if (event->type() == QEvent::MouseMove)
    {
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
    return true;
}
