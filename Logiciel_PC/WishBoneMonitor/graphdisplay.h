#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class GraphDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GraphDisplay(QWidget *parent = 0);

    void UpdateDisplay(QRect);

signals:
    void Delete(GraphDisplay* pReg);

public slots:

private slots:
    void Delete();

private:
    void contextMenuEvent(QContextMenuEvent * event);
    QAction*        m_pDeleteAction;

    QGroupBox       m_GroupBox;
    QGridLayout     m_Layout;

    QwtPlot*        m_pPlot;
    QwtPlotCurve*   m_pCurve;
    double*         m_pX;
    double*         m_pY;

};

#endif // GRAPHDISPLAY_H
