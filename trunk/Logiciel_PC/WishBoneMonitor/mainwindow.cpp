#include "mainwindow.h"
#include "addregisterdialog.h"
#include "wishboneregister.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

MainWindow::MainWindow(WishBoneMonitor *pDoc, QWidget *parent)
    : QMainWindow(parent)
    , m_pDoc(pDoc)
{
    //QDesktopWidget  Desktop;
    //setGeometry(Desktop.screenGeometry());
    setGeometry(100, 100, 800, 500);

    // Barre d'outils
    m_pToolBar = addToolBar("Barre d'outils");
    m_pActionNewConfig          = m_pToolBar->addAction(QIcon("New.png"), "Sauvegarder le configuration", this, SLOT(NewConfig()));
    m_pActionOpenConfig         = m_pToolBar->addAction(QIcon("Open.png"), "Sauvegarder le configuration", this, SLOT(OpenConfig()));
    m_pActionSaveConfig         = m_pToolBar->addAction(QIcon("Save.png"), "Sauvegarder le configuration", this, SLOT(SaveConfig()));
    m_pToolBar->addSeparator();
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
    m_pActionAddRegister        = m_pToolBar->addAction(QIcon("AddRegister.png"), "Ajouter un registre", this, SLOT(AddRegister()));

    // Onglets
    m_pOnglets = new QTabWidget;

        // Onglet 1 : Contrôle des registres WishBone
        m_pControlTab = new ControlTab(m_pDoc);
        m_pOnglets->addTab(m_pControlTab, "Contrôle des registres WishBone");

    setCentralWidget(m_pOnglets);
}

MainWindow::~MainWindow()
{
//    delete m_pControlTab;
//    delete m_pOnglets;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    m_pControlTab->UpdateLayout();

    QWidget::resizeEvent(event);
}

void MainWindow::NewConfig()
{
    m_pDoc->ClearList();
    m_pControlTab->UpdateRegisters();
    m_pControlTab->UpdateLayout();
}

void MainWindow::OpenConfig()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "WishBoneMonitor File (*.wbm)");

    if (!fichier.isEmpty())
    {
        m_pDoc->ClearList();

        QSettings settings(fichier, QSettings::IniFormat);

    // Lecture du groupe [Configuration]
        int NombreDeRegistres = settings.value("Configuration/NombreDeRegistres").toInt();

    // Lecture des groupes [Registre]
        for(int i(0) ; i < NombreDeRegistres ; i++)
        {
            WishBoneRegister* Reg = new WishBoneRegister(settings.value("Registre" + QString::number(i) + "/Nom").toString(),
                                                         settings.value("Registre" + QString::number(i) + "/Adresse").toUInt(),
                                                         settings.value("Registre" + QString::number(i) + "/ValeurMin").toInt(),
                                                         settings.value("Registre" + QString::number(i) + "/ValeurMax").toInt(),
                                                         settings.value("Registre" + QString::number(i) + "/Unite").toString(),
                                                         settings.value("Registre" + QString::number(i) + "/Direction").toString() == "Write" ? true : false,
                                                         settings.value("Registre" + QString::number(i) + "/Periode").toInt());
            m_pDoc->AddRegister(Reg);
        }

        m_pControlTab->UpdateRegisters();
        m_pControlTab->UpdateLayout();
    }
}

void MainWindow::SaveConfig()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "WishBoneMonitor File (*.wbm)");

    if (!fichier.isEmpty())
    {
        QSettings settings(fichier, QSettings::IniFormat);

    // Création du groupe [Configuration]
        settings.beginGroup("Configuration");
        settings.setValue("NombreDeRegistres", QString::number(m_pDoc->GetWishBoneRegisterList()->size()));
        settings.endGroup();

    // Création des groupes [Registre]
        for(int i(0) ; i < m_pDoc->GetWishBoneRegisterList()->size() ; i++)
        {
            settings.beginGroup("Registre" + QString::number(i));

            // Création des différentes clefs et valeurs correspondantes
            settings.setValue("Nom", m_pDoc->GetWishBoneRegisterList()->value(i)->Name());
            settings.setValue("Adresse", QString::number(m_pDoc->GetWishBoneRegisterList()->value(i)->Address()));
            settings.setValue("ValeurMin", QString::number(m_pDoc->GetWishBoneRegisterList()->value(i)->ValueMin()));
            settings.setValue("ValeurMax", QString::number(m_pDoc->GetWishBoneRegisterList()->value(i)->ValueMax()));
            settings.setValue("Unite", m_pDoc->GetWishBoneRegisterList()->value(i)->Unit());
            settings.setValue("Direction", m_pDoc->GetWishBoneRegisterList()->value(i)->Write_nRead() ? "Write" : "Read");
            settings.setValue("Periode", QString::number(m_pDoc->GetWishBoneRegisterList()->value(i)->Period()));

            settings.endGroup();
        }
    }
}

void MainWindow::ConnectSerial()
{
    if (!m_pDoc->GetMailBox()->Connect(m_pComboSerialPort->currentText()))
    {
        QMessageBox::critical(this, "Erreur de connection", "Le port " + m_pComboSerialPort->currentText() + " n'est pas disponnible");
    }

    connect(m_pDoc->GetMailBox()->GetPort(), SIGNAL(readyRead()), this , SLOT(ReceiveSerial()));

    m_pControlTab->UpdateButtons();
}

void MainWindow::DisconnectSerial()
{
    m_pDoc->GetMailBox()->Disconnect();
    m_pControlTab->UpdateButtons();
}

void MainWindow::ReceiveSerial()
{
    WishBoneRegister* TempReg = new WishBoneRegister;
    int IdxTab[16];
    int NbrOfIdx;

    while (m_pDoc->GetMailBox()->DecodeRegister(TempReg))
    {
        NbrOfIdx = m_pDoc->FindRegisters(TempReg->Address(), IdxTab);

        for (int i(0) ; i < NbrOfIdx ; i++)
        {
            m_pDoc->GetWishBoneRegisterList()->value(IdxTab[i])->SetValue(TempReg->Value());
            m_pDoc->GetWishBoneRegisterList()->value(IdxTab[i])->SetDate(TempReg->Date());
        }
    }
    m_pControlTab->UpdateData();

    delete TempReg;
}

void MainWindow::AddRegister()
{
    AddRegisterDialog Dlg(this);
    Dlg.setModal(true);
    if (Dlg.exec() == QDialog::Accepted)
    {
        WishBoneRegister* Reg = new WishBoneRegister(Dlg.Name(),
                                                     Dlg.Address(),
                                                     Dlg.ValueMin(),
                                                     Dlg.ValueMax(),
                                                     Dlg.Unit(),
                                                     Dlg.Write_nRead());
        m_pDoc->AddRegister(Reg);
        m_pControlTab->UpdateRegisters();
        m_pControlTab->UpdateLayout();
    }
}
