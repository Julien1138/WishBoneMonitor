#include "wbgraphdoc.h"

WBGraphDoc::WBGraphDoc(MailBoxDriver* pMailBox)
    : WishBoneWidgetDoc(""
                      , pMailBox
                      , 0
                      , 0
                      , WBGRAPH_WIDTH_MIN
                      , 80)
{
    ClearRegisterList();
}

WBGraphDoc::WBGraphDoc(const QString &Title, MailBoxDriver* pMailBox, int X, int Y, int Width, int Height)
    : WishBoneWidgetDoc(Title
                      , pMailBox
                      , X
                      , Y
                      , Width
                      , Height)
{
    ClearRegisterList();
}

WBGraphDoc::~WBGraphDoc()
{
}

void WBGraphDoc::Load(QSettings *pSettings, QList<WishBoneRegister*>* plistRegisters)
{
    WishBoneWidgetDoc::Load(pSettings, plistRegisters);

    ClearRegisterList();

    int size = pSettings->beginReadArray("Curves");
    for (int i(0) ; i < size ; i++)
    {
        pSettings->setArrayIndex(i);

        int Idx(RegisterIdx(plistRegisters
              , pSettings->value("RegisterAddress").toUInt()
              , pSettings->value("RegisterWrite_nRead").toString() == "true" ? true : false));

        m_RegisterList.push_back(plistRegisters->at(Idx));

        m_CurveNameList.push_back(pSettings->value("CurveName").toString());
    }
    pSettings->endArray();
}

void WBGraphDoc::Save(QSettings *pSettings)
{
    WishBoneWidgetDoc::Save(pSettings);

    pSettings->setValue("Widget", "WBGraph");
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
}

void WBGraphDoc::AddRegister(WishBoneRegister* pRegister, QString CurveName)
{
    m_RegisterList.push_back(pRegister);
    m_CurveNameList.push_back(CurveName);
}

WishBoneRegister* WBGraphDoc::Register(int Idx)
{
    return m_RegisterList.value(Idx);
}

QString WBGraphDoc::CurveName(int Idx)
{
    return m_CurveNameList.value(Idx);
}
