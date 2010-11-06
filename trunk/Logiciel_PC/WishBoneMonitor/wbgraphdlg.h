#ifndef WBGRAPHDLG_H
#define WBGRAPHDLG_H

#include "WishBoneWidgetDlg.h"
#include "WBGraphDoc.h"

#include <QString>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QList>
#include <QGroupBox>

class WBGraphDlg : public WishBoneWidgetDlg
{
    Q_OBJECT
public:
    explicit WBGraphDlg(QList<WishBoneRegister*>* plistRegisters
                      , WBGraphDoc* pDoc
                      , QWidget *parent = 0);

    void UpdateData();

signals:

public slots:
    void    OnAccept();
    void    OnNbrOfCurvesChanged();

private:
    QFormLayout*    m_pNbrOfCurvesLayout;
    QLineEdit*      m_pRunningTimeLineEdit;
    QComboBox*      m_pNbrOfCurvesComboBox;

    QVBoxLayout*        m_pListOfCurvesLayout;
    QList<QFormLayout*> m_pListOfCurveFormLayout;
    QList<QGroupBox*>   m_pListOfCurveGroupBox;
    QList<QComboBox*>   m_pListOfCurveComboBox;
    QList<QLineEdit*>   m_pListOfCurveLineEdit;

    QVBoxLayout m_MainLayout;
    QHBoxLayout m_ButtonsLayout;

    void    UpdateDisplay();
};

#endif // WBGRAPHDLG_H
