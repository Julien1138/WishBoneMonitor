#include "wbwriteregisterview.h"
#include <QVBoxLayout>

WBWriteRegisterView::WBWriteRegisterView(WBWriteRegisterDoc*  pDoc, QWidget *parent)
    : WishBoneWidgetView(pDoc, parent)
    , m_EditValue(this)
    , m_SetButton("Set", this)
{
    m_EditValue.installEventFilter(this);
    m_EditValue.setMouseTracking(true);
    m_EditValue.setEnabled(false);

    m_SetButton.installEventFilter(this);
    m_SetButton.setMouseTracking(true);
    m_SetButton.setEnabled(false);
    connect(&m_SetButton, SIGNAL(clicked()), this, SLOT(WriteRegister()));

    m_pLayout = new QVBoxLayout(this);
    m_pLayout->addWidget(&m_EditValue);
    m_pLayout->addWidget(&m_SetButton);

    m_GroupBox.setLayout(m_pLayout);

    setMinimumSize(WIDTH_MIN, HEIGHT_MIN);

    UpdateDisplay();
}

void WBWriteRegisterView::ModeChanged()
{
    m_EditValue.setDisabled(m_pDoc->ConfigMode());
    m_SetButton.setDisabled(m_pDoc->ConfigMode());
}

void WBWriteRegisterView::WriteRegister()
{
    ((WBWriteRegisterDoc*) m_pDoc)->WriteRegister(m_EditValue.text().toULong(0, 0));
}
