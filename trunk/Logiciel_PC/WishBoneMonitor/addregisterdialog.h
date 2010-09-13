#ifndef ADDREGISTERDIALOG_H
#define ADDREGISTERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>

class AddRegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddRegisterDialog(QWidget *parent = 0);

    QString         Name(){return m_pEditName->text();}
    unsigned long   Address(){return m_pEditAddress->text().toULong(0,0);}
    long            ValueMin(){return m_pEditValueMin->text().toLong(0,0);}
    long            ValueMax(){return m_pEditValueMax->text().toLong(0,0);}
    QString         Unit(){return m_pEditUnit->text();}
    bool            Write_nRead(){return m_pRadioWrite->isChecked();}

signals:

public slots:

private:
    QVBoxLayout*    m_pMainLayout;
    QFormLayout*    m_pFormLayout;
    QLineEdit*      m_pEditName;
    QLineEdit*      m_pEditAddress;
    QLineEdit*      m_pEditValueMin;
    QLineEdit*      m_pEditValueMax;
    QLineEdit*      m_pEditUnit;
    QHBoxLayout*    m_pRadioLayout;
    QLabel*         m_pRadioLabel;
    QRadioButton*   m_pRadioWrite;
    QRadioButton*   m_pRadioRead;
    QHBoxLayout*    m_pButtonsLayout;
    QPushButton*    m_pOKButton;
    QPushButton*    m_pCancelButton;

};

#endif // ADDREGISTERDIALOG_H
