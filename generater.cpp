#include "generater.h"

Generater::Generater()
{

}



QString Generater::numberToText(int n)
{
    QString ZhNum = "零一二三四五六七八九";
    QString result;
    if (n < 10) {
        return QString(ZhNum[n]);
    }

    if (n > 99999999){
        result = numberToText(n/100000000)+"亿";
        n %= 100000000;
    }
    if (n > 9999)
    {
        result += numberToText(n/10000)+"万";
        n %= 10000;
    }
    if (n > 999)
    {
        result += numberToText(n/1000)+"千";
        n %= 1000;
    }
    if (n > 99)
    {
        result += numberToText(n/100)+"百";
        n %= 100;
    }
    if (n > 9)
    {
        result += numberToText(n/10)+"十";
        n %= 10;
    }
   return result;
}

QString Generater::generate(QString arg)
{
    QString result = numberToText(arg.toInt());


    return result;
}
