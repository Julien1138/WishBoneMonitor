#include "registerdisplay.h"
#include <QMessageBox>
#include <QFont>

RegisterDisplay::RegisterDisplay(WishBoneMonitor *pDoc, WishBoneRegister *pRegister, QWidget *parent)
    : QWidget(parent)
    , m_GroupBox(this)
    , m_pDoc(pDoc)
    , m_pRegister(pRegister)
    , m_pPeriodValidator(NULL)
{
    Init();
    Update();
}

RegisterDisplay::RegisterDisplay(const RegisterDisplay & Reg)
    : QWidget(0)
    , m_GroupBox(this)
    , m_pDoc(Reg.m_pDoc)
    , m_pRegister(Reg.m_pRegister)
{
    Init();
    Update();
}

RegisterDisplay& RegisterDisplay::operator=(const RegisterDisplay & Reg)
{
    m_GroupBox.setParent(this);
    m_pDoc = Reg.m_pDoc;
    m_pRegister = Reg.m_pRegister;

    return *this;
}

RegisterDisplay::~RegisterDisplay()
{
    if (m_pPeriodValidator != NULL)
    {
        delete m_pPeriodValidator;
        m_pPeriodValidator = NULL;
    }
    if (m_pValueValidator != NULL)
    {
        delete m_pValueValidator;
        m_pValueValidator = NULL;
    }
}

void RegisterDisplay::Init()
{
    // m_LabelPeriod
    m_LabelPeriod.setText("Periode :");
    m_Layout.addWidget(&m_LabelPeriod, 0, 0);

    // m_EditPeriod
    m_EditPeriod.setFixedWidth(75);
    m_EditPeriod.setAlignment(Qt::AlignRight);
    m_pPeriodValidator = new QIntValidator(0, 65535, this);
    m_EditPeriod.setValidator(m_pPeriodValidator);
    m_Layout.addWidget(&m_EditPeriod, 0, 1);

    // m_LabelPeriodUnit
    m_LabelPeriodUnit.setText("ms");
    m_Layout.addWidget(&m_LabelPeriodUnit, 0, 2);

    // m_LabelValue
    m_LabelValue.setText("Valeur :");
    m_Layout.addWidget(&m_LabelValue, 1, 0);

    // m_EditValue
    m_EditValue.setFixedWidth(75);
    m_EditValue.setAlignment(Qt::AlignRight);
    m_EditValue.setDisabled(!m_pRegister->Write_nRead());
    m_pValueValidator = new QIntValidator(m_pRegister->ValueMin(), m_pRegister->ValueMax(), this);
    m_EditValue.setValidator(m_pValueValidator);
    m_Layout.addWidget(&m_EditValue, 1, 1);

    // m_LabelValueUnit
    m_LabelValueUnit.setText(m_pRegister->Unit());
    m_Layout.addWidget(&m_LabelValueUnit, 1, 2);

    // m_ButtonSend
    m_ButtonSend.setText(m_pRegister->Write_nRead() ? "Write" : "Read");
    m_ButtonSend.setFixedSize(50, 50);
    m_ButtonSend.setEnabled(false);
    m_Layout.addWidget(&m_ButtonSend, 0, 3, 2, 1);
    connect(&m_ButtonSend, SIGNAL(clicked()), this, SLOT(Send()));

    QString sTemp;
    sTemp.setNum(m_pRegister->Address(), 16);
    m_GroupBox.setTitle(m_pRegister->Name() + " : 0x" + sTemp);
    m_GroupBox.setLayout(&m_Layout);
    m_GroupBox.setGeometry(5, 5, 300, 100);

//    QFont Font;
//    Font = m_GroupBox.font();
//    Font.setBold(true);
//    m_GroupBox.setFont(Font);
}

void RegisterDisplay::Update()
{
    QString sTemp;
    sTemp.setNum(m_pRegister->Period());
    m_EditPeriod.setText(sTemp);

    sTemp.setNum(m_pRegister->Value());
    m_EditValue.setText(sTemp);
}

void RegisterDisplay::Send()
{
    m_pRegister->SetValue(m_EditValue.text().toUInt());
    m_pRegister->SetPeriod(m_EditPeriod.text().toUInt());
    m_pDoc->GetMailBox()->SendRegister(m_pRegister);
}

void RegisterDisplay::UpdateReadValue()
{
    if (!m_pRegister->Write_nRead())
    {
        QString sTemp;

        sTemp.setNum(m_pRegister->Value());
        m_EditValue.setText(sTemp);
    }
}
void RegisterDisplay::UpdateButtonEnable()
{
    m_ButtonSend.setEnabled(m_pDoc->GetMailBox()->IsConnected());
}
