#include "wbgraphdoc.h"

WBGraphDoc::WBGraphDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WBGRAPH_WIDTH_MIN
                      , 80)
    , m_RunningTime(10.0)
{
    m_RegisterList.clear();
    m_CurveNameList.clear();
    m_ValueTabList.clear();
    m_DateTabList.clear();
    m_LastDateList.clear();
}

WBGraphDoc::WBGraphDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
    , m_RunningTime(10.0)
{
    m_RegisterList.clear();
    m_CurveNameList.clear();
    m_ValueTabList.clear();
    m_DateTabList.clear();
    m_LastDateList.clear();
}

WBGraphDoc::~WBGraphDoc()
{
    ClearRegisterList();
}

void WBGraphDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    WishBoneWidgetDoc::Load(pSettings, plistRegisters);

    m_RunningTime = pSettings->value("RunningTime").toDouble();

    ClearRegisterList();
    int size = pSettings->beginReadArray("Curves");
    for (int i(0) ; i < size ; i++)
    {
        pSettings->setArrayIndex(i);

        int Idx(RegisterIdx(plistRegisters
              , pSettings->value("RegisterAddress").toUInt()
              , pSettings->value("RegisterWrite_nRead").toString() == "true" ? true : false));

        AddRegister(plistRegisters->at(Idx)
                  , pSettings->value("CurveName").toString());
    }
    pSettings->endArray();
}

void WBGraphDoc::Save(QSettings *pSettings)
{
    WishBoneWidgetDoc::Save(pSettings);

    pSettings->setValue("Widget", "WBGraph");
    pSettings->setValue("RunningTime", QString::number(m_RunningTime));
    pSettings->beginWriteArray("Curves");
    for (int i(0) ; i < NbrOfCurves() ; i++)
    {
        pSettings->setArrayIndex(i);
        pSettings->setValue("RegisterAddress", QString::number(m_RegisterList.value(i)->Address()));
        pSettings->setValue("RegisterWrite_nRead", m_RegisterList.value(i)->Write_nRead() ? "true" : "false");
        pSettings->setValue("CurveName", m_CurveNameList.value(i));
    }
    pSettings->endArray();
}

void WBGraphDoc::ClearRegisterList()
{
    m_RegisterList.clear();
    m_CurveNameList.clear();
    while (!m_ValueTabList.isEmpty())
    {
        delete m_ValueTabList.front();
        m_ValueTabList.pop_front();
    }
    while (!m_DateTabList.isEmpty())
    {
        delete m_DateTabList.front();
        m_DateTabList.pop_front();
    }
    while (!m_LastDateList.isEmpty())
    {
        delete m_LastDateList.front();
        m_LastDateList.pop_front();
    }
}

void WBGraphDoc::AddRegister(WishBoneRegister* pRegister, QString CurveName)
{
    m_RegisterList.push_back(pRegister);
    m_CurveNameList.push_back(CurveName);
    m_ValueTabList.push_back(new QVector<double>);
    m_DateTabList.push_back(new QVector<double>);
    m_LastDateList.push_back(new short);
    *(m_LastDateList.back()) = 0.0;
}

void WBGraphDoc::UdpateTable(int Idx)
{
    m_ValueTabList.value(Idx)->push_back(double((signed long)(m_RegisterList.value(Idx)->Value())));

    short  TimeDiff((short) (m_RegisterList.value(Idx)->Date() - *(m_LastDateList.value(Idx))));
    *(m_LastDateList.value(Idx)) = m_RegisterList.value(Idx)->Date();

    double NextDate(0.0);
    if (m_DateTabList.value(Idx)->isEmpty())
    {
        NextDate = (TimeDiff / 1000.0);
    }
    else
    {
        NextDate = m_DateTabList.value(Idx)->back() + (TimeDiff / 1000.0);
    }
    m_DateTabList.value(Idx)->push_back(NextDate);

    double FirstDate(LatestDate() - m_RunningTime);

    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        if (!(m_DateTabList.value(i)->empty()))
        {
            while (m_DateTabList.value(i)->front() < FirstDate)
            {
                m_DateTabList.value(i)->pop_front();
                m_ValueTabList.value(i)->pop_front();
            }
        }
    }
}

void WBGraphDoc::ResetTables()
{
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        m_ValueTabList.value(i)->clear();
        m_DateTabList.value(i)->clear();
        *(m_LastDateList.value(i)) = 0.0;
    }
}

WishBoneRegister* WBGraphDoc::Register(int Idx)
{
    return m_RegisterList.value(Idx);
}

QString WBGraphDoc::CurveName(int Idx)
{
    return m_CurveNameList.value(Idx);
}

QVector<double>* WBGraphDoc::ValueTab(int Idx)
{
    return m_ValueTabList.value(Idx);
}

QVector<double>* WBGraphDoc::DateTab(int Idx)
{
    return m_DateTabList.value(Idx);
}

double WBGraphDoc::LatestDate()
{
    double LatestDate(0.0);
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        if (!m_DateTabList.value(i)->isEmpty())
        {
            LatestDate = m_DateTabList.value(i)->back() > LatestDate ? m_DateTabList.value(i)->back() : LatestDate;
        }
    }

    return LatestDate;
}
