#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTabWidget>
#include <QToolBar>
#include <QResizeEvent>
#include <QAction>
#include <QComboBox>

#include "wishbonemonitor.h"
#include "paneldoc.h"
#include "panelview.h"
#include "registerlistview.h"
#include "QextSerialBase/qextserialenumerator.h"
#include "QextSerialBase/qextserialport.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WishBoneMonitor *pDoc, QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent* event);

    void RedrawAllTabs();

signals:
    void    ChangeMode();

public slots:
    void    NewConfig();
    void    OpenConfig();
    void    SaveConfig();
    void    ConnectSerial();
    void    DisconnectSerial();
    void    ReceiveSerial();
    void    AddTab();
    void    CloseTab(int i);
    void    ChangeTab(int i);

private:
    QToolBar*   m_pToolBar;
    QAction*    m_pActionNewConfig;
    QAction*    m_pActionOpenConfig;
    QAction*    m_pActionSaveConfig;
    QComboBox*  m_pComboSerialPort;
    QAction*    m_pActionConnectSerial;
    QAction*    m_pActionDisconnectSerial;
    QAction*    m_pActionAddTab;

    QTabWidget* m_pOnglets;

private:
    WishBoneMonitor*    m_pDoc;
    RegisterListView*   m_pRegisterListView;
};

#endif // MAINWINDOW_H
