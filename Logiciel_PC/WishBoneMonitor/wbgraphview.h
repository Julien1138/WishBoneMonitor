#ifndef WBGRAPHVIEW_H
#define WBGRAPHVIEW_H

#include "WishBoneWidgetView.h"
#include "WBGraphDoc.h"
#include "WBGraphDlg.h"

#include <QList>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QColor>

class WBGraphView : public WishBoneWidgetView
{
    Q_OBJECT
public:
    explicit WBGraphView(WBGraphDoc*  pDoc, WBGraphDlg*  pDlg, QWidget *parent = 0);
    ~WBGraphView();

signals:

public slots:
    void    ModeChanged();
    void    Refresh();
    void    UpdateCurve(int Idx);

private:
    QwtPlot*                m_pPlot;
    QList<QwtPlotCurve*>    m_CurveList;

    void    UpdateWidget();

};

QColor CurveColor(int Idx);

#endif // WBGRAPHVIEW_H
