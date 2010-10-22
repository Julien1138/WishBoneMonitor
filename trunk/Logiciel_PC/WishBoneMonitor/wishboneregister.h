#ifndef WISHBONEREGISTER_H
#define WISHBONEREGISTER_H

#include <QString>
#include <QVector>

class WishBoneRegister
{
public:
    WishBoneRegister();
    WishBoneRegister(const WishBoneRegister & Reg);
    WishBoneRegister(QString        Name
                   , unsigned long  Address
                   , long           ValueMin
                   , long           ValueMax
                   , bool           Signed
                   , QString        Unit
                   , bool           Write_nRead
                   , long           Period=0);
    ~WishBoneRegister();

    bool operator==(WishBoneRegister Reg);

    QString         Name() const {return m_Name;}
    unsigned long   Address() const {return m_Address;}
    unsigned long   Value() const {return m_Value;}
    unsigned long   ValueMin() const {return m_ValueMin;}
    unsigned long   ValueMax() const {return m_ValueMax;}
    bool            Signed() const {return m_Signed;}
    QString         Unit() const {return m_Unit;}
    bool            Write_nRead() const {return m_Write_nRead;}
    unsigned long   Period() const {return m_Period;}
    unsigned long   Date() const {return m_Date;}

    void            SetName(QString Name){m_Name = Name;}
    void            SetAddress(unsigned long Address){m_Address = Address;}
    void            SetValue(unsigned long Value);
    void            SetValueNoCheck(unsigned long Value){m_Value = Value;}
    void            SetValueMin(unsigned long ValueMin){m_ValueMin = ValueMin;}
    void            SetValueMax(unsigned long ValueMax){m_ValueMax = ValueMax;}
    void            SetSigned(unsigned long Signed){m_Signed = Signed;}
    void            SetUnit(QString Unit){m_Unit = Unit;}
    void            SetWrite_nRead(bool Write_nRead){m_Write_nRead = Write_nRead;}
    void            SetPeriod(unsigned long Period){m_Period = Period;}
    void            SetDate(unsigned long Date){m_Date = Date;}

    void            UpdateValue(unsigned long Value, unsigned long Date);

private:
    QString         m_Name;
    unsigned long   m_Address;
    unsigned long   m_Value;
    unsigned long   m_ValueMin;
    unsigned long   m_ValueMax;
    bool            m_Signed;
    QString         m_Unit;
    bool            m_Write_nRead;
    unsigned long   m_Period;
    unsigned long   m_Date;
};

#endif // WISHBONEREGISTER_H
