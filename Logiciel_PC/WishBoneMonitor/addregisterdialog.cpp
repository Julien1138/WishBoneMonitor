#include "addregisterdialog.h"

AddRegisterDialog::AddRegisterDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Ajouter un Registre WishBone...");

    m_pMainLayout = new QVBoxLayout;

    m_pFormLayout = new QFormLayout;
    m_pEditName = new QLineEdit;
    m_pFormLayout->addRow("Nom du registre", m_pEditName);
    m_pEditAddress = new QLineEdit;
    m_pFormLayout->addRow("Adresse du registre", m_pEditAddress);
    m_pEditValueMin = new QLineEdit;
    m_pFormLayout->addRow("Valeur max du registre", m_pEditValueMin);
    m_pEditValueMax = new QLineEdit;
    m_pFormLayout->addRow("Valeur min du registre", m_pEditValueMax);
    m_pEditUnit = new QLineEdit;
    m_pFormLayout->addRow("Unité du registre", m_pEditUnit);
    m_pMainLayout->addLayout(m_pFormLayout);

    m_pRadioLayout = new QHBoxLayout;
    m_pRadioLabel = new QLabel("Direction");
    m_pRadioLabel->setFixedWidth(110);
    m_pRadioLayout->addWidget(m_pRadioLabel);
    m_pRadioWrite = new QRadioButton("Ecriture");
    m_pRadioLayout->addWidget(m_pRadioWrite);
    m_pRadioRead = new QRadioButton("Lecture");
    m_pRadioRead->setChecked(true);
    m_pRadioLayout->addWidget(m_pRadioRead);
    m_pMainLayout->addLayout(m_pRadioLayout);

    m_pButtonsLayout = new QHBoxLayout;
    m_pOKButton = new QPushButton("Ajouter");
    m_pButtonsLayout->addWidget(m_pOKButton);
    connect(m_pOKButton, SIGNAL(clicked()), this, SLOT(accept()));
    m_pCancelButton = new QPushButton("Annuler");
    m_pButtonsLayout->addWidget(m_pCancelButton);
    connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    m_pMainLayout->addLayout(m_pButtonsLayout);

    setLayout(m_pMainLayout);
}

