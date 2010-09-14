#ifndef PANELDOC_H
#define PANELDOC_H

#include "wishboneregister.h"
#include <QList>

class PanelDoc
{
public:
    PanelDoc();
    ~PanelDoc();

    QList<WishBoneRegister*>*   GetWishBoneRegisterList(){return & m_listWishBoneRegister;}

    void    AddRegister(WishBoneRegister* Reg);
    void    ClearList();
    int     FindRegisters(unsigned long Address, int* IdxTab);

private:
    QList<WishBoneRegister*>    m_listWishBoneRegister;
};

#endif // PANELDOC_H
