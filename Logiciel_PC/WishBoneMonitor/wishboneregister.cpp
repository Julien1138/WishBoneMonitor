#include "wishboneregister.h"

WishBoneRegister::WishBoneRegister()
    : m_Name("Register")
    , m_Address(0)
    , m_Value(0)
    , m_ValueMin(0)
    , m_ValueMax(0)
    , m_Signed(false)
    , m_Unit("")
    , m_Write_nRead(false)
    , m_Period(0)
    , m_Date(0)
{
    m_ValueTab.clear();
    m_DateTab.clear();
}

WishBoneRegister::WishBoneRegister(const WishBoneRegister & Reg)
    : m_Name(Reg.m_Name)
    , m_Address(Reg.m_Address)
    , m_Value(Reg.m_Value)
    , m_ValueMin(Reg.m_ValueMin)
    , m_ValueMax(Reg.m_ValueMax)
    , m_Signed(Reg.m_Signed)
    , m_Unit(Reg.m_Unit)
    , m_Write_nRead(Reg.m_Write_nRead)
    , m_Period(Reg.m_Period)
    , m_Date(Reg.m_Date)
{
    m_ValueTab.clear();
    m_DateTab.clear();
}

WishBoneRegister::WishBoneRegister(QString          Name
                                 , unsigned long    Address
                                 , long             ValueMin
                                 , long             ValueMax
                                 , bool             Signed
                                 , QString          Unit
                                 , bool             Write_nRead
                                 , long             Period)
    : m_Name(Name)
    , m_Address(Address)
    , m_Value(0)
    , m_ValueMin(ValueMin)
    , m_ValueMax(ValueMax)
    , m_Signed(Signed)
    , m_Unit(Unit)
    , m_Write_nRead(Write_nRead)
    , m_Period(Period)
    , m_Date(0)
{
    m_ValueTab.clear();
    m_DateTab.clear();
}

WishBoneRegister::~WishBoneRegister()
{
}

void WishBoneRegister::SetValue(unsigned long Value)
{
    m_Value = Value;
    if (m_Signed)
    {
        m_ValueTab.push_back(double((signed)Value));
    }
    else
    {
        m_ValueTab.push_back(double((unsigned)Value));
    }
}

void WishBoneRegister::SetDate(unsigned long Date)
{
    m_Date = Date;
    m_DateTab.push_back(Date*0.001);
}
