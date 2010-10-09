#ifndef REGISTERDISPLAY_H
#define REGISTERDISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QAction>

#include "mailboxdriver.h"
#include "wishboneregister.h"

class RegisterDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterDisplay(MailBoxDriver *m_pMailBox, WishBoneRegister *pRegister = 0, QWidget *parent = 0);
    RegisterDisplay(const RegisterDisplay & Reg);
    RegisterDisplay& operator=(const RegisterDisplay & Reg);
    ~RegisterDisplay();

    WishBoneRegister*   Register(){return m_pRegister;}

    void UpdateReadValue();
    void UpdateButtonEnable();

signals:
    void Delete(RegisterDisplay* pReg);
    void AddNew();

public slots:
    void Send();
    void Delete();
    void AddOther();

private:
    void Init();
    void Update();

    void contextMenuEvent(QContextMenuEvent * event);
    QAction*    m_pAddAction;
    QAction*    m_pDeleteAction;

    QGroupBox   m_GroupBox;
    QGridLayout m_Layout;

    MailBoxDriver*      m_pMailBox;
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
