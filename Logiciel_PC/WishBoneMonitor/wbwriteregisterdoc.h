#ifndef WBWRITEREGISTERDOC_H
#define WBWRITEREGISTERDOC_H

#include "WishBoneWidgetDoc.h"
#include "WishBoneRegister.h"

#define WIDTH_MIN   100
#define HEIGHT_MIN  80

class WBWriteRegisterDoc : public WishBoneWidgetDoc
{
public:
    WBWriteRegisterDoc(const QString &Title
                     , MailBoxDriver* pMailBox
                     , int X = 0
                     , int Y = 0
                     , int Width = WIDTH_MIN
                     , int Height = HEIGHT_MIN);
    ~WBWriteRegisterDoc();

    WidgetType GetType(){return eWriteRegister;}

    void    SetpRegister(WishBoneRegister* pReg){m_pRegister = pReg;}
    WishBoneRegister*   Register(){return m_pRegister;}

    void    WriteRegister(unsigned long Value);

private:
    WishBoneRegister*   m_pRegister;
    bool                m_HasSetButton;

};

#endif // WBWRITEREGISTERDOC_H
