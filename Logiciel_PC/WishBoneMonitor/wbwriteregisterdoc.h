#ifndef WBWRITEREGISTERDOC_H
#define WBWRITEREGISTERDOC_H

#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"

#define WIDTH_MIN   100
#define HEIGHT_MIN  40

class WBWriteRegisterDoc : public WishBoneWidgetDoc
{
public:
    WBWriteRegisterDoc(const QString &Title
                     , MailBoxDriver* pMailBox
                     , int X = 0
                     , int Y = 0
                     , int Width = 100
                     , int Height = 80);
    ~WBWriteRegisterDoc();

    WidgetType  GetType(){return eWriteRegister;}
    void        Save(QSettings* pSettings);

    void    SetpRegister(WishBoneRegister* pReg){m_pRegister = pReg;}
    WishBoneRegister*   Register(){return m_pRegister;}

    bool    HasSetButton(){return !(m_pRegister->Period());}

    void    WriteRegister(unsigned long Value);

private:
    WishBoneRegister*   m_pRegister;

};

#endif // WBWRITEREGISTERDOC_H
