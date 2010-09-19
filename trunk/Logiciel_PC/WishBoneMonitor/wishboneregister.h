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

    QString             Name(){return m_Name;}
    unsigned long       Address(){return m_Address;}
    unsigned long       Value(){return m_Value;}
    QVector<double>     ValueTab(){return m_ValueTab;}
    long                ValueMin(){return m_ValueMin;}
    long                ValueMax(){return m_ValueMax;}
    bool                Signed(){return m_Signed;}
    QString             Unit(){return m_Unit;}
    bool                Write_nRead(){return m_Write_nRead;}
    unsigned long       Period(){return m_Period;}
    unsigned long       Date(){return m_Date;}
    QVector<double>     DateTab(){return m_DateTab;}

    void                SetAddress(unsigned long Address){m_Address = Address;}
    void                SetValue(unsigned long Value);
    void                SetPeriod(unsigned long Period){m_Period = Period;}
    void                SetDate(unsigned long Date);

    void                ResetTab();

private:
    QString             m_Name;
    unsigned long       m_Address;
    unsigned long       m_Value;
    QVector<double>     m_ValueTab;
    long                m_ValueMin;
    long                m_ValueMax;
    bool                m_Signed;
    QString             m_Unit;
    bool                m_Write_nRead;
    unsigned long       m_Period;
    unsigned long       m_Date;
    QVector<double>     m_DateTab;
};

#endif // WISHBONEREGISTER_H
