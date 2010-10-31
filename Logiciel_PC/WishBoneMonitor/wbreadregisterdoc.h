#ifndef WBREADREGISTERDOC_H
#define WBREADREGISTERDOC_H

#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"

#define WBREAD_WIDTH_MIN   100
#define WBREAD_HEIGHT_MIN  40

class WBReadRegisterDoc : public WishBoneWidgetDoc
{
public:
    WBReadRegisterDoc(MailBoxDriver* pMailBox);
    WBReadRegisterDoc(const QString &Title
                    , MailBoxDriver* pMailBox
                    , int X = 0
                    , int Y = 0
                    , int Width = WBREAD_WIDTH_MIN
                    , int Height = 80);
    ~WBReadRegisterDoc();

    WidgetType  GetType(){return eReadRegister;}
    void        Load(QSettings* pSettings, QList<WishBoneRegister*>* plistRegisters);
    void        Save(QSettings* pSettings);

    void    SetpRegister(WishBoneRegister* pReg){m_pRegister = pReg;}
    WishBoneRegister*   Register(){return m_pRegister;}

    bool    HasReadButton(){return !(m_pRegister->Period());}

    void    ReadRegister();

private:
    WishBoneRegister*   m_pRegister;
};

#endif // WBREADREGISTERDOC_H
