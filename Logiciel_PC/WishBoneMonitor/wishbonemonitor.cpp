#include "wishbonemonitor.h"

WishBoneMonitor::WishBoneMonitor()
{
    m_pMailBox = new MailBoxDriver();
}

WishBoneMonitor::~WishBoneMonitor()
{
    while (!m_listWishBoneRegister.empty())
    {
        delete m_listWishBoneRegister.front();
        m_listWishBoneRegister.pop_front();
    }
    delete m_pMailBox;
}

void WishBoneMonitor::AddRegister(WishBoneRegister* Reg)
{
    m_listWishBoneRegister.append(Reg);
}

void WishBoneMonitor::ClearList()
{
    while (!m_listWishBoneRegister.empty())
    {
        delete m_listWishBoneRegister.front();
        m_listWishBoneRegister.pop_front();
    }
}

int WishBoneMonitor::FindRegisters(unsigned long Address, int* IdxTab)
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
