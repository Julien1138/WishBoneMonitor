#include "paneldoc.h"

PanelDoc::PanelDoc()
{
    m_listWishBoneRegister.clear();
}

PanelDoc::~PanelDoc()
{
    while (!m_listWishBoneRegister.empty())
    {
        delete m_listWishBoneRegister.front();
        m_listWishBoneRegister.pop_front();
    }
}

void PanelDoc::AddRegister(WishBoneRegister* Reg)
{
    m_listWishBoneRegister.push_back(Reg);
}

void PanelDoc::ClearList()
{
    while (!m_listWishBoneRegister.empty())
    {
        delete m_listWishBoneRegister.front();
        m_listWishBoneRegister.pop_front();
    }
}

int PanelDoc::FindRegisters(unsigned long Address, int* IdxTab)
{
    int NbrOfIdx(0);

    for (int i(0) ; i < m_listWishBoneRegister.size() ; i++)
    {
        if (m_listWishBoneRegister[i]->Address() == Address)
        {
            IdxTab[NbrOfIdx++] = i;
        }
    }

    return NbrOfIdx;
}
