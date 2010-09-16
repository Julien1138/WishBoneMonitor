#include "mainwindow.h"
#include "virtualtab.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QInputDialog>

MainWindow::MainWindow(WishBoneMonitor *pDoc, QWidget *parent)
    : QMainWindow(parent)
    , m_pDoc(pDoc)
{
    setGeometry(100, 100, 800, 500);

    // Barre d'outils principale
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

    m_pActionAddTab             = m_pToolBar->addAction(QIcon("AddTab.png"), "Ajouter un onglet", this, SLOT(AddTab()));
    m_pToolBar->addSeparator();

    // Barre d'outils d'onglet
    m_pPanelToolBar = addToolBar("Barre d'outils Onglet");
    m_pActionAddRegister        = m_pPanelToolBar->addAction(QIcon("AddRegister.png"), "Ajouter un registre", this, SLOT(AddRegister()));

    // Onglets
    m_pOnglets = new QTabWidget;
    m_pOnglets->setTabsClosable(true);
    m_pOnglets->setMovable(true);
    connect(m_pOnglets, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
    connect(m_pOnglets, SIGNAL(currentChanged(int)), this, SLOT(ChangeTab(int)));

    GraphTab* pGraphTab = new GraphTab;
    m_pOnglets->addTab(pGraphTab, "Graph");

    setCentralWidget(m_pOnglets);
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
        {
           ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateLayout();
        }
    }

    QWidget::resizeEvent(event);
}

void MainWindow::NewConfig()
{
    m_pOnglets->clear();
    m_pDoc->ClearList();
}

void MainWindow::OpenConfig()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "WishBoneMonitor File (*.wbm)");

    if (!FileName.isEmpty())
    {
        m_pOnglets->clear();
        m_pDoc->ClearList();

        QSettings settings(FileName, QSettings::IniFormat);

    // Lecture du groupe [Configuration]
        int NombreDePanels = settings.value("Configuration/NombreDePanels").toInt();

        for (int j(0) ; j < NombreDePanels ; j++)
        {
        // Lecture des groupes [Panel]
            if (settings.value("Panel_" + QString::number(j) + "/TypeDePanel").toString() == "Controle")
            {
                PanelDoc* pPanel = new PanelDoc();
                int NombreDeRegistres = settings.value("Panel_" + QString::number(j) + "/NombreDeRegistres").toInt();

            // Lecture des groupes [Registre]
                for(int i(0) ; i < NombreDeRegistres ; i++)
                {
                    WishBoneRegister* Reg = new WishBoneRegister(settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Nom").toString(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Adresse").toUInt(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/ValeurMin").toInt(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/ValeurMax").toInt(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Unite").toString(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Direction").toString() == "Write" ? true : false,
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Periode").toInt());
                    pPanel->AddRegister(Reg);
                }

                m_pDoc->AddPanel(pPanel);
                ControlTab* pControlTab = new ControlTab(m_pDoc->GetMailBox(), m_pDoc->GetPanelList()->last());
                m_pOnglets->addTab(pControlTab,
                                   settings.value("Panel_" + QString::number(j) + "/NomDuPanel").toString());

            }
        }

        if (m_pOnglets->count() > 0)
        {
            // S'il s'agit d'un onglet de contrôle
            if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
            {
               ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateRegisters();
               ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateLayout();
               ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateData();
               ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateButtons();
            }
        }
    }
}

void MainWindow::SaveConfig()
{
    QString FileName = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "WishBoneMonitor File (*.wbm)");

    if (!FileName.isEmpty())
    {
        remove(FileName.toAscii());

        QSettings settings(FileName, QSettings::IniFormat);

    // Création du groupe [Configuration]
        settings.beginGroup("Configuration");
        settings.setValue("NombreDePanels", QString::number(m_pDoc->GetPanelList()->size()));
        settings.endGroup();

        for (int j(0) ; j < m_pOnglets->count() ; j++)
        {
            // S'il s'agit d'un onglet de contrôle
            if (((VirtualTab*)(m_pOnglets->widget(j)))->GetType() == eControlTab)
            {
            // Création des groupes [Panel]
                settings.beginGroup("Panel_" + QString::number(j));
                settings.setValue("TypeDePanel", "Controle");
                settings.setValue("NomDuPanel", m_pOnglets->tabText(j));
                settings.setValue("NombreDeRegistres", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size()));
                settings.endGroup();

            // Création des groupes [Registre]
                for(int i(0) ; i < ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size() ; i++)
                {
                    settings.beginGroup("Registre_" + QString::number(j) + "_" + QString::number(i));

                    // Création des différentes clefs et valeurs correspondantes
                    settings.setValue("Nom", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Name());
                    settings.setValue("Adresse", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Address()));
                    settings.setValue("ValeurMin", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMin()));
                    settings.setValue("ValeurMax", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMax()));
                    settings.setValue("Unite", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Unit());
                    settings.setValue("Direction", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Write_nRead() ? "Write" : "Read");
                    settings.setValue("Periode", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Period()));

                    settings.endGroup();
                }
            }
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

    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
        {
           ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateButtons();
        }
    }
}

void MainWindow::DisconnectSerial()
{
    m_pDoc->GetMailBox()->Disconnect();

    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
        {
           ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateButtons();
        }
    }
}

void MainWindow::ReceiveSerial()
{
    WishBoneRegister* TempReg = new WishBoneRegister;
    int IdxTab[16];
    int NbrOfIdx;

    while (m_pDoc->GetMailBox()->DecodeRegister(TempReg))
    {
        for (int j(0) ; j < m_pDoc->GetPanelList()->size(); j++)
        {
            NbrOfIdx = m_pDoc->GetPanelList()->value(j)->FindRegisters(TempReg->Address(), IdxTab);

            for (int i(0) ; i < NbrOfIdx ; i++)
            {
                m_pDoc->GetPanelList()->value(j)->GetWishBoneRegisterList()->value(IdxTab[i])->SetValue(TempReg->Value());
                m_pDoc->GetPanelList()->value(j)->GetWishBoneRegisterList()->value(IdxTab[i])->SetDate(TempReg->Date());
            }
        }
    }

    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
        {
           ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateData();
        }
    }

    delete TempReg;
}

void MainWindow::AddRegister()
{
    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
        {
           ((ControlTab*)(m_pOnglets->currentWidget()))->AddRegister();
        }
    }
}

void MainWindow::AddTab()
{
    bool ok(false);

    QStringList PanelTypeList;
    PanelTypeList << "Contrôle" << "Graphe";
    QString PanelType = QInputDialog::getItem(this, "Type de panel", "Quel type de panel\nsouhaitez vous ajouter ?", PanelTypeList, 0, false, &ok);

    if (ok)
    {
        QString PanelName = QInputDialog::getText(this, "Nom du Panel", "Quel nom souhaites vous\ndonner à votre panel ?", QLineEdit::Normal, QString(), &ok);

        if (ok)
        {
            if (PanelType == "Contrôle")
            {
                PanelDoc* pPanel = new PanelDoc();
                m_pDoc->AddPanel(pPanel);
                ControlTab* pControlTab = new ControlTab(m_pDoc->GetMailBox(), m_pDoc->GetPanelList()->last());
                m_pOnglets->addTab(pControlTab, PanelName);
            }
            else if (PanelType == "Graphe")
            {
            }
        }
    }
}

void MainWindow::CloseTab(int i)
{
    // S'il s'agit d'un onglet de contrôle
    if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eControlTab)
    {
        m_pDoc->GetPanelList()->removeOne(((ControlTab*)(m_pOnglets->widget(i)))->GetPanel());
        m_pOnglets->removeTab(i);
    }
}

void MainWindow::ChangeTab(int i)
{
    if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contrôle
        if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eControlTab)
        {
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateRegisters();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateLayout();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateData();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateButtons();
        }
    }
}
