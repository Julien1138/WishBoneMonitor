#ifndef WBGRAPHVIEW_H
#define WBGRAPHVIEW_H

#include "WishBoneWidgetView.h"
#include "WBGraphDoc.h"
#include "WBGraphDlg.h"

#include <QList>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

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

private:
    QwtPlot*    m_pPlot;

    void    UpdateWidget();

};

#endif // WBGRAPHVIEW_H
