#include "wbgraphdlg.h"

WBGraphDlg::WBGraphDlg(QList<WishBoneRegister*>* plistRegisters, WBGraphDoc* pDoc, QWidget *parent)
    : WishBoneWidgetDlg(plistRegisters, pDoc, parent)
    , m_MainLayout()
    , m_ButtonsLayout()
{
    m_pLayout = new QVBoxLayout();

    m_pNbrOfCurvesLayout = new QFormLayout();
    m_pNbrOfCurvesLayout->addRow("Titre du Widget", &m_EditTitle);
    m_pRunningTimeLineEdit = new QLineEdit();
    m_pNbrOfCurvesLayout->addRow("Profondeur mémoire", m_pRunningTimeLineEdit);
    m_pRunningTimeLineEdit->setText(QString::number(((WBGraphDoc*) m_pDoc)->RunningTime()));
    m_pNbrOfCurvesComboBox = new QComboBox();
    m_pNbrOfCurvesComboBox->addItem("0");
    m_pNbrOfCurvesComboBox->addItem("1");
    m_pNbrOfCurvesComboBox->addItem("2");
    m_pNbrOfCurvesComboBox->addItem("3");
    m_pNbrOfCurvesComboBox->addItem("4");
    m_pNbrOfCurvesComboBox->addItem("5");
    connect(m_pNbrOfCurvesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnNbrOfCurvesChanged()));
    m_pNbrOfCurvesLayout->addRow("Nombre de Courbes", m_pNbrOfCurvesComboBox);
    ((QVBoxLayout*) m_pLayout)->addLayout(m_pNbrOfCurvesLayout);

    m_pListOfCurvesLayout = new QVBoxLayout();
    ((QVBoxLayout*) m_pLayout)->addLayout(m_pListOfCurvesLayout);

    m_MainLayout.addLayout(m_pLayout);

    connect(&m_OKButton, SIGNAL(clicked()), this, SLOT(OnAccept()));
    m_ButtonsLayout.addWidget(&m_OKButton);
    m_ButtonsLayout.addWidget(&m_CancelButton);
    m_MainLayout.addLayout(&m_ButtonsLayout);

    setLayout(&m_MainLayout);
}

void WBGraphDlg::OnAccept()
{
    WishBoneWidgetDlg::OnAccept();

    ((WBGraphDoc*) m_pDoc)->SetRunningTime(m_pRunningTimeLineEdit->text().toDouble());

    ((WBGraphDoc*) m_pDoc)->ClearRegisterList();
    for (int i(0) ; i < m_pNbrOfCurvesComboBox->currentText().toInt() ; i++)
    {
        ((WBGraphDoc*) m_pDoc)->AddRegister(m_plistRegisters->value(m_pListOfCurveComboBox.value(i)->currentIndex())
                                          , m_pListOfCurveLineEdit.value(i)->text());
    }
}

void WBGraphDlg::OnNbrOfCurvesChanged()
{
    UpdateDisplay();
}

void WBGraphDlg::UpdateData()
{
    WishBoneWidgetDlg::UpdateData();

    m_pRunningTimeLineEdit->setText(QString::number(((WBGraphDoc*) m_pDoc)->RunningTime()));

    m_pNbrOfCurvesComboBox->setCurrentIndex(((WBGraphDoc*) m_pDoc)->NbrOfCurves());

    UpdateDisplay();
}

void WBGraphDlg::UpdateDisplay()
{
    while (!m_pListOfCurveComboBox.isEmpty())
    {
        delete m_pListOfCurveComboBox.front();
        m_pListOfCurveComboBox.pop_front();
    }

    while (!m_pListOfCurveLineEdit.isEmpty())
    {
        delete m_pListOfCurveLineEdit.front();
        m_pListOfCurveLineEdit.pop_front();
    }

    while (!m_pListOfCurveFormLayout.isEmpty())
    {
        delete m_pListOfCurveFormLayout.front();
        m_pListOfCurveFormLayout.pop_front();
    }

    while (!m_pListOfCurveGroupBox.isEmpty())
    {
        delete m_pListOfCurveGroupBox.front();
        m_pListOfCurveGroupBox.pop_front();
    }

    for(int i(0) ; i < m_pNbrOfCurvesComboBox->currentText().toInt() ; i++)
    {
        QGroupBox* pGroupBox = new QGroupBox();
        pGroupBox->setTitle("Graph n°" + QString::number(i));
        m_pListOfCurveGroupBox.push_back(pGroupBox);
        m_pListOfCurvesLayout->addWidget(pGroupBox);

        QFormLayout* pLayout = new QFormLayout();
        m_pListOfCurveFormLayout.push_back(pLayout);
        pGroupBox->setLayout(pLayout);

        QComboBox* pComboBox = new QComboBox();
        for (int i(0) ; i < m_plistRegisters->count() ; i++)
        {
            //if (!(m_plistRegisters->value(i)->Write_nRead()))
            {
                pComboBox->addItem(m_plistRegisters->value(i)->Name());
            }
        }
        pLayout->addRow("Nom du registre", pComboBox);
        m_pListOfCurveComboBox.push_back(pComboBox);

        QLineEdit* pLineEdit = new QLineEdit();
        pLayout->addRow("Titre de la courbe", pLineEdit);
        m_pListOfCurveLineEdit.push_back(pLineEdit);
    }

    for (int i(0) ; i < m_pNbrOfCurvesComboBox->currentText().toInt() ; i++)
    {
        int CurrentIndex(m_plistRegisters->indexOf(((WBGraphDoc*) m_pDoc)->Register(i)));
        m_pListOfCurveComboBox.value(i)->setCurrentIndex(CurrentIndex == -1 ? 0 : CurrentIndex);
        m_pListOfCurveLineEdit.value(i)->setText(((WBGraphDoc*) m_pDoc)->CurveName(i));
    }
}
