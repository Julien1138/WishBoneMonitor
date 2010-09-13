#ifndef WISHBONEREGISTER_H
#define WISHBONEREGISTER_H

#include <QString>

class WishBoneRegister
{
public:
    WishBoneRegister();
    WishBoneRegister(const WishBoneRegister & Reg);
    WishBoneRegister(QString        Name
                   , unsigned long  Address
                   , long           ValueMin
                   , long           ValueMax
                   , QString        Unit
                   , bool           Write_nRead
                   , long           Period=0);
    ~WishBoneRegister();

    QString         Name(){return m_Name;}
    unsigned long   Address(){return m_Address;}
    long            Value(){return m_Value;}
    long            ValueMin(){return m_ValueMin;}
    long            ValueMax(){return m_ValueMax;}
    QString         Unit(){return m_Unit;}
    bool            Write_nRead(){return m_Write_nRead;}
    long            Period(){return m_Period;}
    long            Date(){return m_Date;}

    void            SetAddress(unsigned long Address){m_Address = Address;}
    void            SetValue(long Value){m_Value = Value;}
    void            SetPeriod(long Period){m_Period = Period;}
    void            SetDate(long Date){m_Date = Date;}

private:
    QString         m_Name;
    unsigned long   m_Address;
    long            m_Value;
    long            m_ValueMin;
    long            m_ValueMax;
    QString         m_Unit;
    bool            m_Write_nRead;
    long            m_Period;
    long            m_Date;
};

#endif // WISHBONEREGISTER_H
