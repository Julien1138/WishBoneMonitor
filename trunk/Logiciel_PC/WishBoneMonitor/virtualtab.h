#ifndef VIRTUALTAB_H
#define VIRTUALTAB_H

#include <QWidget>

enum TabType{eControlTab, eGraphTab};

class VirtualTab : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualTab(QWidget *parent = 0);
    virtual TabType GetType() = 0;

signals:

public slots:

};

#endif // VIRTUALTAB_H
