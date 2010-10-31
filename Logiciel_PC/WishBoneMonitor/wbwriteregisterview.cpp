#include "wbwriteregisterview.h"
#include <QGridLayout>

WBWriteRegisterView::WBWriteRegisterView(WBWriteRegisterDoc*  pDoc, WBWriteRegisterDlg*  pDlg, QWidget *parent)
    : WishBoneWidgetView(pDoc, pDlg, parent)
    , m_EditValue(this)
    , m_pSetButton(NULL)
{
    m_pLayout = new QGridLayout();

    m_EditValue.installEventFilter(this);
    m_EditValue.setMouseTracking(true);
    m_EditValue.setEnabled(false);
    ((QGridLayout*) m_pLayout)->addWidget(&m_EditValue, 0, 0);

    m_LabelUnit.setText(((WBWriteRegisterDoc*) m_pDoc)->Register()->Unit());
    ((QGridLayout*) m_pLayout)->addWidget(&m_LabelUnit, 0, 1);

    if (((WBWriteRegisterDoc*) m_pDoc)->HasSetButton())
    {
        m_pSetButton = new QPushButton("Set", this);
        m_pSetButton->installEventFilter(this);
        m_pSetButton->setMouseTracking(true);
        m_pSetButton->setEnabled(false);
        connect(m_pSetButton, SIGNAL(clicked()), this, SLOT(WriteRegister()));
        ((QGridLayout*) m_pLayout)->addWidget(m_pSetButton, 1, 0, 1, 2);
    }

    m_GroupBox.setLayout(m_pLayout);

    setMinimumSize(WBWRITE_WIDTH_MIN, WBWRITE_HEIGHT_MIN);

    UpdateDisplay();

    connect(((WBWriteRegisterDoc*) m_pDoc)->Register(), SIGNAL(UpdateWidget()), this, SLOT(Refresh()));
}

void WBWriteRegisterView::ModeChanged()
{
    m_EditValue.setDisabled(m_pDoc->ConfigMode() || !((WBWriteRegisterDoc*) m_pDoc)->Register()->Write_nRead());
    if (m_pSetButton)
    {
        m_pSetButton->setDisabled(m_pDoc->ConfigMode() || !((WBWriteRegisterDoc*) m_pDoc)->Register()->Write_nRead());
    }
}

void WBWriteRegisterView::WriteRegister()
{
    if (((WBWriteRegisterDoc*) m_pDoc)->Register()->Signed())
    {
        ((WBWriteRegisterDoc*) m_pDoc)->WriteRegister((unsigned long)(m_EditValue.text().toLong(0, 0)));
    }
    else
    {
        ((WBWriteRegisterDoc*) m_pDoc)->WriteRegister(m_EditValue.text().toULong(0, 0));
    }

}

void WBWriteRegisterView::Refresh()
{
    QString sTemp;
    if (((WBWriteRegisterDoc*) m_pDoc)->Register()->Signed())
    {
        sTemp.setNum((signed long) (((WBWriteRegisterDoc*) m_pDoc)->Register()->Value()));
    }
    else
    {
        sTemp.setNum((unsigned long) (((WBWriteRegisterDoc*) m_pDoc)->Register()->Value()));

    }
    m_EditValue.setText(sTemp);

    m_LabelUnit.setText(((WBWriteRegisterDoc*) m_pDoc)->Register()->Unit());
}

void WBWriteRegisterView::UpdateWidget()
{
    WishBoneWidgetView::UpdateWidget();

    if (((WBWriteRegisterDoc*) m_pDoc)->HasSetButton())
    {
        if (!m_pSetButton)
        {
            m_pSetButton = new QPushButton("Set", this);
            m_pSetButton->installEventFilter(this);
            m_pSetButton->setMouseTracking(true);
            m_pSetButton->setEnabled(false);
            connect(m_pSetButton, SIGNAL(clicked()), this, SLOT(WriteRegister()));
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
}
