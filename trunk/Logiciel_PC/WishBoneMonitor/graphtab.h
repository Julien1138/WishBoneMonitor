#ifndef GRAPHTAB_H
#define GRAPHTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include "virtualtab.h"
#include "graphdisplay.h"

class GraphTab : public VirtualTab
{
    Q_OBJECT
public:
    explicit GraphTab(QWidget *parent = 0);
    ~GraphTab();

    bool UpdateLayout();

    TabType     GetType(){return eGraphTab;}

signals:

public slots:
    void AddGraph();
    void DelGraph(GraphDisplay*);

private:
    QVBoxLayout             m_Layout;
    QList<GraphDisplay*>    m_listGraphDisplay;

    void contextMenuEvent(QContextMenuEvent * event);

};

#endif // GRAPHTAB_H
