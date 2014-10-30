#include "generator.h"

QString Generator::generate(QString leadingText)
{
    table.clear();
    buffer.clear();
    deadlocked = false;
    QString result = leadingText;
    addBuffer(result);
    result = buffer;

    return result;
}

void Generator::addBuffer(QString text)
{
    int loop, len = text.length();
    for (loop = 0; loop < len; ++loop) {
        QChar ch = text[loop];
        ushort chid = ch.unicode();
        if (0x3200 < chid && chid < 0x9fff)
            buffer.append(ch);
    }
}

QString Generator::numberToText(int n)
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

Generator::Generator() {
    templateLeadingLines.append(QString("在这句话中"));
    templateLeadingLines.append("本句中");
    templateLeadingLines.append("在此句中");

    templateLines.append("共有%1个汉字");
    templateLines.append("总计%1个汉字");
    templateLines.append("一共有%1个字");
}
