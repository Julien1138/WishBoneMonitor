#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTabWidget>
#include <QToolBar>
#include <QResizeEvent>
#include <QAction>
#include <QComboBox>

#include "controltab.h"
#include "wishbonemonitor.h"
#include "QextSerialBase/qextserialenumerator.h"
#include "QextSerialBase/qextserialport.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WishBoneMonitor *pDoc, QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent* event);

public slots :
    void    NewConfig();
    void    OpenConfig();
    void    SaveConfig();
    void    AddRegister();
    void    ConnectSerial();
    void    DisconnectSerial();
    void    ReceiveSerial();

private:
    QToolBar*   m_pToolBar;
    QAction*    m_pActionNewConfig;
    QAction*    m_pActionOpenConfig;
    QAction*    m_pActionSaveConfig;
    QComboBox*  m_pComboSerialPort;
    QAction*    m_pActionConnectSerial;
    QAction*    m_pActionDisconnectSerial;
    QAction*    m_pActionAddRegister;

    QTabWidget* m_pOnglets;
    ControlTab* m_pControlTab;

private:
    WishBoneMonitor*    m_pDoc;
};

#endif // MAINWINDOW_H
