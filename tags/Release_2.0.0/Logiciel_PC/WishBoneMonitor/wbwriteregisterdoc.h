#ifndef WBWRITEREGISTERDOC_H
#define WBWRITEREGISTERDOC_H

#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"

#define WBWRITE_WIDTH_MIN   100
#define WBWRITE_HEIGHT_MIN  80

class WBWriteRegisterDoc : public WishBoneWidgetDoc
{
public:
    WBWriteRegisterDoc(MailBoxDriver* pMailBox);
    WBWriteRegisterDoc(const QString &Title
                     , MailBoxDriver* pMailBox
                     , int X = 0
                     , int Y = 0
                     , int Width = WBWRITE_WIDTH_MIN
                     , int Height = WBWRITE_HEIGHT_MIN);
    ~WBWriteRegisterDoc();

    WidgetType  GetType(){return eWriteRegister;}
    void        Load(QSettings* pSettings, QList<WishBoneRegister*>* plistRegisters);
    void        Save(QSettings* pSettings);

    void    SetpRegister(WishBoneRegister* pReg){m_pRegister = pReg;}
    WishBoneRegister*   Register(){return m_pRegister;}

    bool    HasSetButton(){return !(m_pRegister->Period());}

    void    WriteRegister(unsigned long Value);

private:
    WishBoneRegister*   m_pRegister;

};

#endif // WBWRITEREGISTERDOC_H
