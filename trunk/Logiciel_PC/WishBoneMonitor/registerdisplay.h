#ifndef REGISTERDISPLAY_H
#define REGISTERDISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>

#include "wishbonemonitor.h"
#include "wishboneregister.h"

class RegisterDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterDisplay(WishBoneMonitor *pDoc, WishBoneRegister *pRegister = 0, QWidget *parent = 0);
    RegisterDisplay(const RegisterDisplay & Reg);
    RegisterDisplay& operator=(const RegisterDisplay & Reg);
    ~RegisterDisplay();

    void UpdateReadValue();
    void UpdateButtonEnable();

signals:

public slots:
    void Send();

private:
    void Init();
    void Update();

    QGroupBox   m_GroupBox;
    QGridLayout m_Layout;

    WishBoneMonitor*    m_pDoc;
    WishBoneRegister*   m_pRegister;
    QLabel      m_LabelPeriod;
    QLineEdit   m_EditPeriod;
    QValidator* m_pPeriodValidator;
    QLabel      m_LabelPeriodUnit;
    QPushButton m_ButtonSend;
    QLabel      m_LabelValue;
    QLineEdit   m_EditValue;
    QValidator* m_pValueValidator;
    QLabel      m_LabelValueUnit;
};

#endif // REGISTERDISPLAY_H
