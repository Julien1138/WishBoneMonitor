#ifndef WBGRAPHDOC_H
#define WBGRAPHDOC_H

#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"
#include <QList>
#include <QVector>

#define WBGRAPH_WIDTH_MIN   350
#define WBGRAPH_HEIGHT_MIN  200

class WBGraphDoc : public WishBoneWidgetDoc
{
public:
    WBGraphDoc(MailBoxDriver* pMailBox);
    WBGraphDoc(const QString &Title
             , MailBoxDriver* pMailBox
             , int X = 0
             , int Y = 0
             , int Width = WBGRAPH_WIDTH_MIN
             , int Height = WBGRAPH_HEIGHT_MIN);
    ~WBGraphDoc();

    WidgetType  GetType(){return eGraph;}
    void        Load(QSettings* pSettings, QList<WishBoneRegister*>* plistRegisters);
    void        Save(QSettings* pSettings);

    void                ClearRegisterList();
    void                AddRegister(WishBoneRegister* pRegister, QString CurveName);
    void                UdpateTable(int Idx);
    void                ResetTables();
    WishBoneRegister*   Register(int Idx);
    QString             CurveName(int Idx);
    QVector<double>*    ValueTab(int Idx);
    QVector<double>*    DateTab(int Idx);
    int                 NbrOfCurves(){return m_RegisterList.count();}

    void    SetRunningTime(double RunningTime){m_RunningTime = RunningTime;}
    double  RunningTime(){return m_RunningTime;}

private:
    QList<WishBoneRegister*>    m_RegisterList;
    QList<QString>              m_CurveNameList;
    QList<QVector<double>*>     m_ValueTabList;
    QList<QVector<double>*>     m_DateTabList;
    QList<short*>               m_LastDateList;
    double                      m_RunningTime;

    double  LatestDate();
};

#endif // WBGRAPHDOC_H
