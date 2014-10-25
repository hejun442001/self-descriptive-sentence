#include "generater.h"

Generater::Generater()
{

}


QString Generater::numberToText(int n)
{
    if (n == 0) return "零";
    const QString numberName = "零一二三四五六七八九";
    const QString unitName = "十百千";
    const QString groupUnitName = "万亿";
    QString result;
    bool lessThenZero = false;
    bool hideZeroFlag = false;
    bool groupUnitFlag = false;
    int bit = 0;

    if (n < 0) {
        lessThenZero = true;
        n = -n;
    }

    while (n) {
        int x = n % 10;
        n /= 10;
        if ((bit % 4) == 0) groupUnitFlag = true;
        if (groupUnitFlag) hideZeroFlag = true;

        if (!hideZeroFlag) result = QString(numberName[x])
                +((bit%4)>0 ? QString(unitName[(bit%4)-1]) : QString())
                +(groupUnitFlag&&(bit/4)>0?QString(groupUnitName[(bit/4)-1]):QString())
                +result;


        hideZeroFlag = (x == 0);
        groupUnitFlag = groupUnitFlag && hideZeroFlag;
        ++bit;
    }

    if (lessThenZero) result = "负" + result;
    return result;
}

QString Generater::generate(QString arg)
{
    // @TODO: Finish it
    QString result = numberToText(arg.toInt());


    return result;
}
