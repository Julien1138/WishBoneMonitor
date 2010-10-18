#include "mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QInputDialog>

MainWindow::MainWindow(WishBoneMonitor *pDoc, QWidget *parent)
    : QMainWindow(parent)
    , m_pDoc(pDoc)
{
    setGeometry(100, 100, 850, 500);

// Barre d'outils principale
    // Gestion du document
    m_pToolBar = addToolBar("Barre d'outils");
    m_pActionNewConfig          = m_pToolBar->addAction(QIcon("New.png"), "Sauvegarder le configuration", this, SLOT(NewConfig()));
    m_pActionOpenConfig         = m_pToolBar->addAction(QIcon("Open.png"), "Sauvegarder le configuration", this, SLOT(OpenConfig()));
    m_pActionSaveConfig         = m_pToolBar->addAction(QIcon("Save.png"), "Sauvegarder le configuration", this, SLOT(SaveConfig()));
    m_pToolBar->addSeparator();
    // Gestion de la connexion s�rie
    m_pComboSerialPort          = new QComboBox();
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    for (int i(0) ; i < ports.size() ; i++)
    {
        m_pComboSerialPort->addItem(QString(ports.at(i).portName.toLocal8Bit().constData()));
    }
    connect(m_pComboSerialPort, SIGNAL(currentIndexChanged(int)), this , SLOT(DisconnectSerial()));
    m_pToolBar->addWidget(m_pComboSerialPort);
    m_pActionConnectSerial      = m_pToolBar->addAction(QIcon("Connect.png"), "Connecter au port s�rie", this, SLOT(ConnectSerial()));
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
//    if (m_pOnglets->count() > 0)
//    {
        // S'il s'agit d'un onglet de contr�le
//        if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eControlTab)
//        {
//           ((ControlTab*)(m_pOnglets->currentWidget()))->UpdateLayout();
//        }
        // S'il s'agit d'un onglet de graphe
//        else if (((VirtualTab*)(m_pOnglets->currentWidget()))->GetType() == eGraphTab)
//        {
//           ((GraphTab*)(m_pOnglets->currentWidget()))->UpdateLayout();
//        }
//    }

    QWidget::resizeEvent(event);
}

void MainWindow::NewConfig()
{
    m_pOnglets->clear();
    m_pDoc->ClearRegisterList();
    m_pDoc->ClearPanelList();
}

void MainWindow::OpenConfig()
{
    /*QString FileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "WishBoneMonitor File (*.wbm)");

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
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Signed").toString() == "Signed" ? true : false,
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
            else if (settings.value("Panel_" + QString::number(j) + "/TypeDePanel").toString() == "Graphe")
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
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Signed").toString() == "Signed" ? true : false,
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Unite").toString(),
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Direction").toString() == "Write" ? true : false,
                                                                 settings.value("Registre_" + QString::number(j) + "_" + QString::number(i) + "/Periode").toInt());
                    pPanel->AddRegister(Reg);
                }

                m_pDoc->AddPanel(pPanel);
                GraphTab* pControlTab = new GraphTab(m_pDoc->GetMailBox(), m_pDoc->GetPanelList()->last());
                int Nombre_de_Graphs = settings.value("Panel_" + QString::number(j) + "/NombreDeGraphs").toInt();
                for (int i(0) ; i < Nombre_de_Graphs ; i++)
                {
                    pControlTab->GetpListNbrOfCurves()->push_back(settings.value("Panel_" + QString::number(j) + "/NombreDeCourbes_Graphe_" + QString::number(i)).toInt());
                }
                m_pOnglets->addTab(pControlTab,
                                   settings.value("Panel_" + QString::number(j) + "/NomDuPanel").toString());

            }
        }

        ChangeTab(0);
    }*/
}

void MainWindow::SaveConfig()
{
    /*QString FileName = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "WishBoneMonitor File (*.wbm)");

    if (!FileName.isEmpty())
    {
    // On supprime le fichier de sauvegarde s'il existe d�ja
        remove(FileName.toAscii());

    // On cr�e un nouveau fichier de sauvegarde
        QSettings settings(FileName, QSettings::IniFormat);

    // Cr�ation du groupe [Configuration]
        settings.beginGroup("Configuration");
        settings.setValue("NombreDePanels", QString::number(m_pDoc->GetPanelList()->size()));
        settings.endGroup();

        for (int j(0) ; j < m_pOnglets->count() ; j++)
        {
            // S'il s'agit d'un onglet de contr�le
            if (((VirtualTab*)(m_pOnglets->widget(j)))->GetType() == eControlTab)
            {
            // Cr�ation des groupes [Panel]
                settings.beginGroup("Panel_" + QString::number(j));
                settings.setValue("TypeDePanel", "Controle");
                settings.setValue("NomDuPanel", m_pOnglets->tabText(j));
                settings.setValue("NombreDeRegistres", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size()));
                settings.endGroup();

            // Cr�ation des groupes [Registre]
                for(int i(0) ; i < ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size() ; i++)
                {
                    settings.beginGroup("Registre_" + QString::number(j) + "_" + QString::number(i));

                    // Cr�ation des diff�rentes clefs et valeurs correspondantes
                    settings.setValue("Nom", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Name());
                    settings.setValue("Adresse", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Address()));
                    settings.setValue("ValeurMin", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMin()));
                    settings.setValue("ValeurMax", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMax()));
                    settings.setValue("Signed", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Signed() ? "Signed" : "Unsigned");
                    settings.setValue("Unite", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Unit());
                    settings.setValue("Direction", ((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Write_nRead() ? "Write" : "Read");
                    settings.setValue("Periode", QString::number(((ControlTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Period()));

                    settings.endGroup();
                }
            }
            // S'il s'agit d'un onglet de graphe
            else if (((VirtualTab*)(m_pOnglets->widget(j)))->GetType() == eGraphTab)
            {
            // Cr�ation des groupes [Panel]
                settings.beginGroup("Panel_" + QString::number(j));
                settings.setValue("TypeDePanel", "Graphe");
                settings.setValue("NomDuPanel", m_pOnglets->tabText(j));
                settings.setValue("NombreDeRegistres", QString::number(((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size()));
                settings.setValue("NombreDeGraphs", ((GraphTab*)(m_pOnglets->widget(j)))->GetpListNbrOfCurves()->size());
                for (int i(0) ; i < ((GraphTab*)(m_pOnglets->widget(j)))->GetpListNbrOfCurves()->size() ; i++)
                {
                    settings.setValue("NombreDeCourbes_Graphe_" + QString::number(i), ((GraphTab*)(m_pOnglets->widget(j)))->GetpListNbrOfCurves()->value(i));
                }
                settings.endGroup();

            // Cr�ation des groupes [Registre]
                for(int i(0) ; i < ((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->size() ; i++)
                {
                    settings.beginGroup("Registre_" + QString::number(j) + "_" + QString::number(i));

                    // Cr�ation des diff�rentes clefs et valeurs correspondantes
                    settings.setValue("Nom", ((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Name());
                    settings.setValue("Adresse", QString::number(((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Address()));
                    settings.setValue("ValeurMin", QString::number(((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMin()));
                    settings.setValue("ValeurMax", QString::number(((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->ValueMax()));
                    settings.setValue("Signed", ((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Signed() ? "Signed" : "Unsigned");
                    settings.setValue("Unite", ((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Unit());
                    settings.setValue("Direction", ((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Write_nRead() ? "Write" : "Read");
                    settings.setValue("Periode", QString::number(((GraphTab*)(m_pOnglets->widget(j)))->GetPanel()->GetWishBoneRegisterList()->value(i)->Period()));

                    settings.endGroup();
                }
            }
        }
    }*/
}

void MainWindow::ConnectSerial()
{
    if (!m_pDoc->GetMailBox()->Connect(m_pComboSerialPort->currentText()))
    {
        QMessageBox::critical(this, "Erreur de connection", "Le port " + m_pComboSerialPort->currentText() + " n'est pas disponnible");
    }
    else
    {
        connect(m_pDoc->GetMailBox()->GetPort(), SIGNAL(readyRead()), this , SLOT(ReceiveSerial()));
        m_pDoc->SetConnectMode();
        emit ChangeMode();
    }
    m_pRegisterListView->UpdateDisplay();
}

void MainWindow::DisconnectSerial()
{
    m_pDoc->GetMailBox()->Disconnect();

    if (!(m_pDoc->GetMailBox()->IsConnected()))
    {
        m_pDoc->SetConfigMode();
        emit ChangeMode();
    }
    m_pRegisterListView->UpdateDisplay();
}

void MainWindow::ReceiveSerial()
{
    WishBoneRegister* TempReg = new WishBoneRegister;
//    int IdxTab[16];
//    int NbrOfIdx;

    while (m_pDoc->GetMailBox()->DecodeRegister(TempReg))
    {
        m_pDoc->GetRegisterList()->value(m_pDoc->GetRegisterList()->indexOf(TempReg))->UpdateValue(TempReg->Value(), TempReg->Date());
    }

    /*if (m_pOnglets->count() > 0)
    {
        for (int i(0) ; i < m_pOnglets->count() ; i++)
        {
            // S'il s'agit d'un onglet de contr�le
            if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eControlTab)
            {
               ((ControlTab*)(m_pOnglets->widget(i)))->UpdateData();
            }
            // S'il s'agit d'un onglet de graphe
            else if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eGraphTab)
            {
               ((GraphTab*)(m_pOnglets->widget(i)))->UpdateData();
            }
        }
    }*/

    delete TempReg;
}

void MainWindow::AddTab()
{
    bool ok(false);

    QString PanelName = QInputDialog::getText(this, "Nom du Panel", "Quel nom souhaites vous\ndonner � votre panel ?", QLineEdit::Normal, "Nouveau Panel", &ok);

    if (ok)
    {
        PanelView*  Panel = new PanelView(m_pDoc->AddPanel(PanelName));
        connect(this, SIGNAL(ChangeMode()), Panel, SLOT(ModeChanged()));
        int Idx = m_pOnglets->addTab(Panel, Panel->pDoc()->Title());
        m_pOnglets->setCurrentIndex(Idx);
    }
}

void MainWindow::CloseTab(int i)
{
    if (i != m_pOnglets->indexOf(m_pRegisterListView))
    {
        m_pDoc->DelPanel((PanelDoc*)(m_pOnglets->widget(i)));
        m_pOnglets->removeTab(i);
    }
}

void MainWindow::ChangeTab(int i)
{
    /*if (m_pOnglets->count() > 0)
    {
        // S'il s'agit d'un onglet de contr�le
        if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eControlTab)
        {
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateRegisters();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateLayout();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateData();
            ((ControlTab*)(m_pOnglets->widget(i)))->UpdateButtons();
        }
        // S'il s'agit d'un onglet de contr�le
        if (((VirtualTab*)(m_pOnglets->widget(i)))->GetType() == eGraphTab)
        {
            ((GraphTab*)(m_pOnglets->widget(i)))->UpdateRegisters();
            ((GraphTab*)(m_pOnglets->widget(i)))->UpdateLayout();
            ((GraphTab*)(m_pOnglets->widget(i)))->UpdateData();
            ((GraphTab*)(m_pOnglets->widget(i)))->UpdateButtons();
        }
    }*/
}
