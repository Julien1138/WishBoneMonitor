#include "registerlistview.h"
#include "addregisterdialog.h"
#include "wishboneregister.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QRect>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSignalMapper>

RegisterListView::RegisterListView(WishBoneMonitor* pDoc, QWidget *parent) :
    QWidget(parent),
    m_pDoc(pDoc)
{
    m_Table.setColumnCount(10);
    QStringList HLabels;
    HLabels << "Adresse" << "Nom" << "Echelle" << "Unité" << "Signe" << "Valeur Min" << "Valeur Max" << "Direction" << "Periode (ms)" << "";
    m_Table.setHorizontalHeaderLabels(HLabels);

    m_Layout.addWidget(&m_Table, 0, 0);
    setLayout(&m_Layout);

    connect(&m_Table, SIGNAL(cellChanged(int,int)),
            this    , SLOT(ModifyRegister(int, int)));

    UpdateDisplay();
}

void RegisterListView::UpdateDisplay()
{
    m_UpdateInProgress = true;

    m_Table.setRowCount(m_pDoc->GetRegisterList()->count());

    QSignalMapper *mapper = new QSignalMapper();

    for (int i(0) ; i < m_pDoc->GetRegisterList()->count() ; i++)
    {
        int k(0);

        // Adresse
        m_Table.setItem(i, k, new QTableWidgetItem("0x" + QString::number(m_pDoc->GetRegisterList()->value(i)->Address(), 16)));
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Nom
        m_Table.setItem(i, k, new QTableWidgetItem(m_pDoc->GetRegisterList()->value(i)->Name()));
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Coefficient d'échelle
        m_Table.setItem(i, k, new QTableWidgetItem(QString::number((double) m_pDoc->GetRegisterList()->value(i)->ScaleCoefficient())));
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Unité
        m_Table.setItem(i, k, new QTableWidgetItem(m_pDoc->GetRegisterList()->value(i)->Unit()));
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Signe
        QComboBox* pComboSigne = new QComboBox;
        pComboSigne->addItem("Signé");
        pComboSigne->addItem("Non Signé");
        m_Table.setCellWidget(i, k, pComboSigne);
        pComboSigne->setCurrentIndex(m_pDoc->GetRegisterList()->value(i)->Signed() ? 0 : 1);
        pComboSigne->setEnabled(m_pDoc->ConfigMode());
        connect(pComboSigne, SIGNAL(currentIndexChanged(int)), mapper, SLOT(map()));
        mapper->setMapping(pComboSigne, i);
        connect(pComboSigne, SIGNAL(currentIndexChanged(int)), this, SLOT(ModifySignBox(int)));
        k++;

        // Valeur Min
        if (m_pDoc->GetRegisterList()->value(i)->Signed())
        {
            m_Table.setItem(i, k, new QTableWidgetItem(QString::number((signed long) (m_pDoc->GetRegisterList()->value(i)->ValueMin()))));
        }
        else
        {
            m_Table.setItem(i, k, new QTableWidgetItem(QString::number((unsigned long) (m_pDoc->GetRegisterList()->value(i)->ValueMin()))));
        }
        if (!m_pDoc->GetRegisterList()->value(i)->Write_nRead())
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Valeur Max
        if (m_pDoc->GetRegisterList()->value(i)->Signed())
        {
            m_Table.setItem(i, k, new QTableWidgetItem(QString::number((signed long) (m_pDoc->GetRegisterList()->value(i)->ValueMax()))));
        }
        else
        {
            m_Table.setItem(i, k, new QTableWidgetItem(QString::number((unsigned long) (m_pDoc->GetRegisterList()->value(i)->ValueMax()))));
        }
        if (!m_pDoc->GetRegisterList()->value(i)->Write_nRead())
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Direction
        QComboBox* pComboDir = new QComboBox;
        pComboDir->addItem("Lecture");
        pComboDir->addItem("Ecriture");
        m_Table.setCellWidget(i, k, pComboDir);
        pComboDir->setCurrentIndex(m_pDoc->GetRegisterList()->value(i)->Write_nRead() ? 1 : 0);
        pComboDir->setEnabled(m_pDoc->ConfigMode());
        connect(pComboDir, SIGNAL(currentIndexChanged(int)), mapper, SLOT(map()));
        mapper->setMapping(pComboDir, i);
        connect(pComboDir, SIGNAL(currentIndexChanged(int)), this, SLOT(ModifyDirectionBox(int)));
        k++;

        // Periode
        m_Table.setItem(i, k, new QTableWidgetItem(QString::number(m_pDoc->GetRegisterList()->value(i)->Period())));
        if (m_pDoc->GetRegisterList()->value(i)->Write_nRead())
        {
            m_pDoc->GetRegisterList()->value(i)->SetPeriod(0);
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
            m_Table.item(i, k)->setText(QString::number(m_pDoc->GetRegisterList()->value(i)->Period()));
        }
        if (!(m_pDoc->ConfigMode()))
        {
            m_Table.item(i, k)->setFlags(m_Table.item(i, k)->flags() & ~Qt::ItemIsEnabled);
        }
        k++;

        // Bouton supprimer
        QPushButton* pDelButton = new QPushButton("Supprimer");
        m_Table.setCellWidget(i, k, pDelButton);
        connect(pDelButton, SIGNAL(pressed()), mapper, SLOT(map()));
        mapper->setMapping(pDelButton, i);
        connect(pDelButton, SIGNAL(clicked()), this, SLOT(DelReg()));
        k++;
    }

    connect(mapper, SIGNAL(mapped(int)), &m_Table, SLOT(selectRow(int)));

    m_UpdateInProgress = false;
}

void RegisterListView::AddReg()
{
    AddRegisterDialog Dlg(true, this);
    Dlg.setModal(true);
    if (Dlg.exec() == QDialog::Accepted)
    {
        WishBoneRegister Reg(Dlg.Name(),
                             Dlg.Address(),
                             Dlg.ValueMin(),
                             Dlg.ValueMax(),
                             Dlg.Signed(),
                             Dlg.ScaleCoefficient(),
                             Dlg.Unit(),
                             Dlg.Write_nRead(),
                             Dlg.Periode());

        if (!m_pDoc->AddRegister(Reg))
        {
            QMessageBox::warning(this, "Ajout d'un registre", "Attention, ce registre existe déjà !");
        }
        UpdateDisplay();
    }
}

void RegisterListView::DelReg()
{
    m_pDoc->DelRegister(m_Table.currentRow());
    UpdateDisplay();
}

void RegisterListView::contextMenuEvent(QContextMenuEvent * event)
{
    if (m_pDoc->ConfigMode())
    {
        if (event->x() > m_Table.x() && event->x() < (m_Table.x() + m_Table.width()) &&
            event->y() > m_Table.y() && event->y() < (m_Table.y() + m_Table.height()))
        {
            QMenu * menu = new QMenu(this);
            menu->addAction("Ajouter un registre", this, SLOT(AddReg()));

            menu->exec(event->globalPos());
        }
    }
}

void RegisterListView::ModifyRegister(int currentRow, int currentColumn)
{
    if (!m_UpdateInProgress)
    {
        if (currentColumn == 0)
        {
            if (m_pDoc->RegisterExists(m_Table.item(currentRow, currentColumn)->text().toULong(0,0), m_pDoc->GetRegisterList()->value(currentRow)->Write_nRead()))
            {
                QMessageBox::warning(this, "Ajout d'un registre", "Attention, ce registre existe déjà !");
                m_Table.setCurrentCell(currentRow, currentColumn);
            }
            else
            {
                m_pDoc->GetRegisterList()->value(currentRow)->SetAddress(m_Table.item(currentRow, currentColumn)->text().toULong(0,0));
            }
        }
        else if (currentColumn == 1)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetName(m_Table.item(currentRow, currentColumn)->text());
        }
        else if (currentColumn == 2)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetScaleCoefficient(m_Table.item(currentRow, currentColumn)->text().toDouble());
        }
        else if (currentColumn == 3)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetUnit(m_Table.item(currentRow, currentColumn)->text());
        }
        else if (currentColumn == 5)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetValueMin(m_Table.item(currentRow, currentColumn)->text().toLong(0,0));
        }
        else if (currentColumn == 6)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetValueMax(m_Table.item(currentRow, currentColumn)->text().toLong(0,0));
        }
        else if (currentColumn == 8)
        {
            m_pDoc->GetRegisterList()->value(currentRow)->SetPeriod(m_Table.item(currentRow, currentColumn)->text().toLong(0,0));
        }

        UpdateDisplay();
    }
}

void RegisterListView::ModifySignBox(int Idx)
{
    m_pDoc->GetRegisterList()->value(m_Table.currentRow())->SetSigned(Idx == 0 ? true : false);
    UpdateDisplay();
}

void RegisterListView::ModifyDirectionBox(int Idx)
{
    if (m_pDoc->RegisterExists(m_pDoc->GetRegisterList()->value(m_Table.currentRow())->Address(),
                               Idx == 0 ? false : true))
    {
        QMessageBox::warning(this, "Ajout d'un registre", "Attention, ce registre existe déjà !");
    }
    else
    {
        m_pDoc->GetRegisterList()->value(m_Table.currentRow())->SetWrite_nRead(Idx == 0 ? false : true);
    }
    UpdateDisplay();
}
