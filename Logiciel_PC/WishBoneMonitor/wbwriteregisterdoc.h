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

    WidgetType GetType(){return eWriteRegister;}

    void    SetpRegister(WishBoneRegister* pReg){m_pRegister = pReg;}
    WishBoneRegister*   Register(){return m_pRegister;}

    void    SetHasSetButton(bool HasSetButton){m_HasSetButton = HasSetButton;}
    bool    HasSetButton(){return m_HasSetButton;}

    void    WriteRegister(unsigned long Value);

private:
    WishBoneRegister*   m_pRegister;
    bool                m_HasSetButton;

};

#endif // WBWRITEREGISTERDOC_H
