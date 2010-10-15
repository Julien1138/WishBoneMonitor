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
#include <QComboBox>

class AddRegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddRegisterDialog(bool Writable, QWidget *parent = 0);

    QString         Name(){return m_pEditName->text();}
    unsigned long   Address(){return m_pEditAddress->text().toULong(0,0);}
    bool            Signed(){return (m_pComboSign->currentText() == "Signé");}
    long            ValueMin(){return m_pEditValueMin->text().toLong(0,0);}
    long            ValueMax(){return m_pEditValueMax->text().toLong(0,0);}
    QString         Unit(){return m_pEditUnit->text();}
    bool            Write_nRead(){return m_pRadioWrite->isChecked();}
    unsigned long   Periode(){return m_pEditPeriode->text().toULong(0,0);}

signals:

public slots:

private slots:
    void            ReadWriteChange();

private:
    QVBoxLayout*    m_pMainLayout;
    QFormLayout*    m_pFormLayout;
    QLineEdit*      m_pEditName;
    QLineEdit*      m_pEditAddress;
    QLineEdit*      m_pEditValueMin;
    QLineEdit*      m_pEditValueMax;
    QLineEdit*      m_pEditUnit;
    QLineEdit*      m_pEditPeriode;
    QHBoxLayout*    m_pRadioLayout;
    QLabel*         m_pRadioLabel;
    QRadioButton*   m_pRadioWrite;
    QRadioButton*   m_pRadioRead;
    QHBoxLayout*    m_pButtonsLayout;
    QPushButton*    m_pOKButton;
    QPushButton*    m_pCancelButton;
    QComboBox*      m_pComboSign;

};

#endif // ADDREGISTERDIALOG_H
