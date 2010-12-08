#ifndef REGISTERLISTVIEW_H
#define REGISTERLISTVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QBoxLayout>
#include "wishbonemonitor.h"

class RegisterListView : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterListView(WishBoneMonitor* pDoc, QWidget *parent = 0);

    void UpdateDisplay();

signals:

public slots:
    void AddReg();
    void DelReg();
    void ModifyRegister(int currentRow, int currentColumn);
    void ModifySignBox(int Idx);
    void ModifyDirectionBox(int Idx);

private:
    QTableWidget        m_Table;
    QGridLayout         m_Layout;
    WishBoneMonitor*    m_pDoc;

    void contextMenuEvent(QContextMenuEvent * event);

    bool                m_UpdateInProgress;

};

#endif // REGISTERLISTVIEW_H
