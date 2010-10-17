#ifndef WISHBONEWIDGETDOC_H
#define WISHBONEWIDGETDOC_H

#include <QString>

enum WidgetType{eWriteRegister, eReadRegister};

class WishBoneWidgetDoc
{
public:
    WishBoneWidgetDoc(const QString &Title, bool* pConnectedMode, int X, int Y, int Width, int Height);
    ~WishBoneWidgetDoc();

    virtual WidgetType GetType() = 0;

    QString Title(){return m_Title;}
    bool    ConfigMode(){return !(*m_pConnectedMode);}
    int     X(){return m_X;}
    int     Y(){return m_Y;}
    int     Width(){return m_Width;}
    int     Height(){return m_Height;}

    void    SetTitle(QString Title){m_Title = Title;}
    void    SetX(int X){m_X = X;}
    void    SetY(int Y){m_Y = Y;}
    void    SetWidth(int Width){m_Width = Width;}
    void    SetHeight(int Height){m_Height = Height;}

protected:
    QString m_Title;
    bool*   m_pConnectedMode;

private:
    int     m_X;
    int     m_Y;
    int     m_Width;
    int     m_Height;
};

#endif // WISHBONEWIDGETDOC_H
