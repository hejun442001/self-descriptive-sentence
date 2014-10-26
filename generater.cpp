#include "generater.h"

Generater::Generater()
{

}


QString Generater::numberToText(int n)
{

    const QString numberName = "零一二三四五六七八九";
    const QString unitName = "十百千";
    const QString groupUnitName = "万亿";
    QString result;
    bool lessThenZero = false;
    bool zeroFlag = true;
    bool groupUnitFlag = false;
    int bit = 0;

    if (n < 0) {
        lessThenZero = true;
        n = -n;
    }
    else if (n == 0)
        return numberName[0];

    while (n) {
        int x = n % 10;
        n /= 10;
        groupUnitFlag = (groupUnitFlag && zeroFlag) || ((bit % 4) == 0);


        if (x != 0)
            result = QString(numberName[x])
                    +((bit%4)>0 ? unitName[(bit%4)-1] : QString())
                    +(groupUnitFlag&&(bit/4)>0?groupUnitName[(bit/4)-1]:QString())
                    +result;
        else if (!zeroFlag)
            result = QString(numberName[0]) + result;


        zeroFlag =  (x == 0);
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
