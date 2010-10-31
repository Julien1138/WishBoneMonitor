#include "mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QInputDialog>
#include <QApplication>

MainWindow::MainWindow(WishBoneMonitor *pDoc, QWidget *parent)
    : QMainWindow(parent)
    , m_pDoc(pDoc)
{
    setGeometry(80, 100, 900, 500);

// Barre d'outils principale
    // Gestion du document
    m_pToolBar = addToolBar("Barre d'outils");
    m_pActionNewConfig          = m_pToolBar->addAction(QIcon("New.png"), "Sauvegarder le configuration", this, SLOT(NewConfig()));
    m_pActionOpenConfig         = m_pToolBar->addAction(QIcon("Open.png"), "Sauvegarder le configuration", this, SLOT(OpenConfig()));
    m_pActionSaveConfig         = m_pToolBar->addAction(QIcon("Save.png"), "Sauvegarder le configuration", this, SLOT(SaveConfig()));
    m_pToolBar->addSeparator();
    // Gestion de la connexion série
    m_pComboSerialPort          = new QComboBox();
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    for (int i(0) ; i < ports.size() ; i++)
    {
        m_pComboSerialPort->addItem(QString(ports.at(i).portName.toLocal8Bit().constData()));
    }
    connect(m_pComboSerialPort, SIGNAL(currentIndexChanged(int)), this , SLOT(DisconnectSerial()));
    m_pToolBar->addWidget(m_pComboSerialPort);
    m_pActionConnectSerial      = m_pToolBar->addAction(QIcon("Connect.png"), "Connecter au port série", this, SLOT(ConnectSerial()));
    m_pActionDisconnectSerial   = m_pToolBar->addAction(QIcon("Disconnect.png"), "Deconnecter", this, SLOT(DisconnectSerial()));
    m_pToolBar->addSeparator();
    // Gestion des onglets
    m_pActionAddTab             = m_pToolBar->addAction(QIcon("AddTab.png"), "Ajouter un onglet", this, SLOT(AddTab()));

// Onglets
    // Parametrage des onglets
    m_pOnglets = new QTabWidget;
    m_pOnglets->setTabsClosable(true);
    m_pOnglets->setMovable(true);
    connect(m_pOnglets, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
    connect(m_pOnglets, SIGNAL(currentChanged(int)), this, SLOT(ChangeTab(int)));
    setCentralWidget(m_pOnglets);
    // Onglet "Table des registres"
    m_pRegisterListView = new RegisterListView(m_pDoc);
    m_pOnglets->addTab(m_pRegisterListView, "Table des Registres");
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
}

void MainWindow::NewConfig()
{
    if (!m_pDoc->GetMailBox()->IsConnected())
    {
        m_pOnglets->clear();
        m_pDoc->ClearRegisterList();
        m_pDoc->ClearPanelList();

        m_pOnglets->addTab(m_pRegisterListView, "Table des Registres");
        m_pRegisterListView->UpdateDisplay();
    }
}

void MainWindow::OpenConfig()
{
    if (!m_pDoc->GetMailBox()->IsConnected())
    {
        QApplication::setOverrideCursor( Qt::WaitCursor ); // changer de curseur

        NewConfig();

        QString FileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "WishBoneMonitor File (*.wbm)");

        if (!FileName.isEmpty())
        {
        // On ouvre le fichier de sauvegarde
            QSettings settings(FileName, QSettings::IniFormat);

            m_pDoc->Load(&settings);
        }
        RedrawAllTabs();

        QApplication::restoreOverrideCursor(); // restaurer le curseur
    }
}

void MainWindow::SaveConfig()
{
    if (!m_pDoc->GetMailBox()->IsConnected())
    {
        QString FileName = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "WishBoneMonitor File (*.wbm)");

        if (!FileName.isEmpty())
        {
        // On supprime le fichier de sauvegarde s'il existe déja
            remove(FileName.toAscii());

        // On crée un nouveau fichier de sauvegarde
            QSettings settings(FileName, QSettings::IniFormat);

            m_pDoc->Save(&settings);
        }
    }
}

void MainWindow::ConnectSerial()
{
    QApplication::setOverrideCursor( Qt::WaitCursor ); // changer de curseur

    if (!m_pDoc->GetMailBox()->Connect(m_pComboSerialPort->currentText()))
    {
        QMessageBox::critical(this, "Erreur de connection", "Le port " + m_pComboSerialPort->currentText() + " n'est pas disponnible");
    }
    else
    {
        connect(m_pDoc->GetMailBox()->GetPort(), SIGNAL(readyRead()), this , SLOT(ReceiveSerial()));
        emit ChangeMode();
        m_pRegisterListView->UpdateDisplay();
        m_pDoc->SetupMailBox();
    }

    QApplication::restoreOverrideCursor(); // restaurer le curseur
}

void MainWindow::DisconnectSerial()
{
    QApplication::setOverrideCursor( Qt::WaitCursor ); // changer de curseur

    m_pDoc->GetMailBox()->Disconnect();

    if (!(m_pDoc->GetMailBox()->IsConnected()))
    {
        emit ChangeMode();
    }
    m_pRegisterListView->UpdateDisplay();

    QApplication::restoreOverrideCursor(); // restaurer le curseur
}

void MainWindow::ReceiveSerial()
{
    WishBoneRegister* TempReg = new WishBoneRegister;

    while (m_pDoc->GetMailBox()->DecodeRegister(TempReg))
    {
        int Idx(m_pDoc->RegisterIdx(TempReg->Address(), TempReg->Write_nRead()));
        if (Idx != -1)
        {
            m_pDoc->GetRegisterList()->value(Idx)->UpdateValue(TempReg->Value(), TempReg->Date());
        }
    }

    delete TempReg;
}

void MainWindow::AddTab()
{
    if (!m_pDoc->GetMailBox()->IsConnected())
    {
        bool ok(false);

        QString PanelName = QInputDialog::getText(this, "Nom du Panel", "Quel nom souhaites vous\ndonner à votre panel ?", QLineEdit::Normal, "Nouveau Panel", &ok);

        if (ok)
        {
            m_pDoc->AddPanel(PanelName);
            RedrawAllTabs();
            m_pOnglets->setCurrentIndex(m_pDoc->GetPanelList()->count());
        }
    }
}

void MainWindow::CloseTab(int i)
{
    if (!m_pDoc->GetMailBox()->IsConnected())
    {
        if (i != m_pOnglets->indexOf(m_pRegisterListView))
        {
            m_pDoc->DelPanel((PanelDoc*)(m_pOnglets->widget(i)));
            m_pOnglets->removeTab(i);
        }
    }
}

void MainWindow::ChangeTab(int i)
{
}

void MainWindow::RedrawAllTabs()
{
    m_pOnglets->clear();

    m_pOnglets->addTab(m_pRegisterListView, "Table des Registres");
    m_pRegisterListView->UpdateDisplay();

    for (int i(0) ; i < m_pDoc->GetPanelList()->count() ; i++)
    {
        PanelView* Panel = new PanelView(m_pDoc->GetPanelList()->value(i));
        connect(this, SIGNAL(ChangeMode()), Panel, SLOT(ModeChanged()));
        m_pOnglets->addTab(Panel, Panel->pDoc()->Title());
        Panel->RedrawAllWidgets();
    }
}
