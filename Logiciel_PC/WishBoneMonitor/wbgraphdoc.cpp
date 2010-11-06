#include "wbgraphdoc.h"

WBGraphDoc::WBGraphDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WBGRAPH_WIDTH_MIN
                      , 80)
    , m_RunningTime(60.0)
{
    m_RegisterList.clear();
    m_CurveNameList.clear();
    m_ValueTabList.clear();
    m_DateTabList.clear();
}

WBGraphDoc::WBGraphDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
    , m_RunningTime(60.0)
{
    m_RegisterList.clear();
    m_CurveNameList.clear();
    m_ValueTabList.clear();
    m_DateTabList.clear();
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
}

void WBGraphDoc::AddRegister(WishBoneRegister* pRegister, QString CurveName)
{
    m_RegisterList.push_back(pRegister);
    m_CurveNameList.push_back(CurveName);
    m_ValueTabList.push_back(new QVector<double>);
    m_DateTabList.push_back(new QVector<double>);
}

void WBGraphDoc::UdpateTables()
{
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        double Date(m_RegisterList.value(i)->Date()/1000.0);
        if ((m_DateTabList.value(i)->isEmpty()) ||
            (Date != m_DateTabList.value(i)->back()))
        {
            if (m_RegisterList.value(i)->Signed())
            {
                m_ValueTabList.value(i)->push_back(double((signed)(m_RegisterList.value(i)->Value())));
            }
            else
            {
                m_ValueTabList.value(i)->push_back(double((unsigned)(m_RegisterList.value(i)->Value())));
            }

            if (!m_DateTabList.value(i)->isEmpty())
            {
                while (Date < m_DateTabList.value(i)->back())
                {
                    Date += (double(0x10000) / 1000);
                }
            }
            m_DateTabList.value(i)->push_back(Date);
        }
    }

    double LatestDate(0.0);
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        LatestDate = m_DateTabList.value(i)->back() > LatestDate ? m_DateTabList.value(i)->back() : LatestDate;
    }

    double FirstDate(LatestDate - m_RunningTime);
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        while ((!m_DateTabList.value(i)->isEmpty()) &&
               (m_DateTabList.value(i)->front() < FirstDate))
        {
            m_ValueTabList.value(i)->pop_front();
            m_DateTabList.value(i)->pop_front();
        }
    }
}

void WBGraphDoc::ResetTables()
{
    for (int i(0) ; i < m_RegisterList.count() ; i++)
    {
        m_ValueTabList.value(i)->clear();
        m_DateTabList.value(i)->clear();
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
