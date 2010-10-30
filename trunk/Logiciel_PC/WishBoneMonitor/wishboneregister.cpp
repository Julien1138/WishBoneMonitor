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
}

WishBoneRegister::~WishBoneRegister()
{
}

void WishBoneRegister::Load(QSettings *pSettings)
{
    m_Name =        pSettings->value("Name").toString();
    m_Address =     pSettings->value("Address").toUInt();
    m_ValueMin =    pSettings->value("ValueMin").toUInt();
    m_ValueMax =    pSettings->value("ValueMax").toUInt();
    m_Signed =      pSettings->value("Signed").toString() == "true" ? true : false;
    m_Unit =        pSettings->value("Unit").toString();
    m_Write_nRead = pSettings->value("Write_nRead").toString() == "true" ? true : false;
    m_Period =      pSettings->value("Period").toUInt();
}

void WishBoneRegister::Save(QSettings *pSettings)
{
    pSettings->setValue("Name", m_Name);
    pSettings->setValue("Address", QString::number(m_Address));
    pSettings->setValue("ValueMin", QString::number(m_ValueMin));
    pSettings->setValue("ValueMax", QString::number(m_ValueMax));
    pSettings->setValue("Signed", m_Signed ? "true" : "false");
    pSettings->setValue("Unit", m_Unit);
    pSettings->setValue("Write_nRead", m_Write_nRead ? "true" : "false");
    pSettings->setValue("Period", QString::number(m_Period));
}

bool WishBoneRegister::operator ==(WishBoneRegister Reg)
{
    return (m_Address == Reg.Address() &&
            m_Write_nRead == Reg.Write_nRead());
}

void WishBoneRegister::UpdateValue(unsigned long Value, unsigned long Date)
{
    m_Value = Value;
    m_Date = Date;
    emit UpdateWidget();
}

void WishBoneRegister::SetValue(unsigned long Value)
{
    if (m_Signed)
    {
        if (((signed long) Value) > ((signed long) (m_ValueMax)))
        {
           m_Value = m_ValueMax;
        }
        else if (((signed long) Value) < ((signed long) (m_ValueMin)))
        {
            m_Value = m_ValueMin;
        }
        else
        {
            m_Value = Value;
        }
    }
    else
    {
        if (((unsigned long) Value) > ((unsigned long) (m_ValueMax)))
        {
            m_Value = m_ValueMax;
        }
        else if (((unsigned long) Value) < ((unsigned long) (m_ValueMin)))
        {
            m_Value = m_ValueMin;
        }
        else
        {
            m_Value = Value;
        }
    }
}
