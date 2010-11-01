#include "wbreadregisterview.h"
#include <QGridLayout>

WBReadRegisterView::WBReadRegisterView(WBReadRegisterDoc*  pDoc, WBReadRegisterDlg*  pDlg, QWidget *parent)
    : WishBoneWidgetView(pDoc, pDlg, parent)
    , m_EditValue(this)
    , m_pSetButton(NULL)
{
    m_pLayout = new QGridLayout();

    m_EditValue.installEventFilter(this);
    m_EditValue.setMouseTracking(true);
    m_EditValue.setEnabled(false);
    ((QGridLayout*) m_pLayout)->addWidget(&m_EditValue, 0, 0);

    ((QGridLayout*) m_pLayout)->addWidget(&m_LabelUnit, 0, 1);

    UpdateWidget();

    m_GroupBox.setLayout(m_pLayout);

    setMinimumSize(WBREAD_WIDTH_MIN, WBREAD_HEIGHT_MIN);

    UpdateDisplay();
}

void WBReadRegisterView::ModeChanged()
{
    m_EditValue.setDisabled(m_pDoc->ConfigMode() || ((WBReadRegisterDoc*) m_pDoc)->Register()->Write_nRead());
    if (m_pSetButton)
    {
        m_pSetButton->setDisabled(m_pDoc->ConfigMode() || ((WBReadRegisterDoc*) m_pDoc)->Register()->Write_nRead());
    }
}

void WBReadRegisterView::ReadRegister()
{
    ((WBReadRegisterDoc*) m_pDoc)->ReadRegister();
}

void WBReadRegisterView::Refresh()
{
    QString sTemp;
    if (((WBReadRegisterDoc*) m_pDoc)->Register()->Signed())
    {
        sTemp.setNum((signed long) (((WBReadRegisterDoc*) m_pDoc)->Register()->Value()));
    }
    else
    {
        sTemp.setNum((unsigned long) (((WBReadRegisterDoc*) m_pDoc)->Register()->Value()));

    }
    m_EditValue.setText(sTemp);

    m_LabelUnit.setText(((WBReadRegisterDoc*) m_pDoc)->Register()->Unit());
}

void WBReadRegisterView::UpdateWidget()
{
    WishBoneWidgetView::UpdateWidget();

    if (((WBReadRegisterDoc*) m_pDoc)->Register())
    {
        m_LabelUnit.setText(((WBReadRegisterDoc*) m_pDoc)->Register()->Unit());

        if (((WBReadRegisterDoc*) m_pDoc)->HasReadButton())
        {
            if (!m_pSetButton)
            {
                m_pSetButton = new QPushButton("Read", this);
                m_pSetButton->installEventFilter(this);
                m_pSetButton->setMouseTracking(true);
                m_pSetButton->setEnabled(false);
                connect(m_pSetButton, SIGNAL(clicked()), this, SLOT(ReadRegister()));
                ((QGridLayout*) m_pLayout)->addWidget(m_pSetButton, 1, 0, 1, 2);
            }
        }
        else
        {
            if (m_pSetButton)
            {
                delete m_pSetButton;
                m_pSetButton = NULL;
            }
        }

        connect(((WBReadRegisterDoc*) m_pDoc)->Register(), SIGNAL(UpdateWidget()), this, SLOT(Refresh()));
    }
}
