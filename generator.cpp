#include "generator.h"


void Generator::generate(QString leadingText)
{
    table.clear();
    buffer.clear();
    charCounter = 0;
    if (leadingText.isEmpty()) {
        leadingText = templateLeadingLines[qrand()%templateLeadingLines.size()];
    } else {
        leadingText.remove("。");
    }
    QString appendText = templateLines[qrand()%templateLines.size()];

    QStringList result;
    result << leadingText
           << appendText
           << templateDetails;
    addString(result.join(""));
    addString(numberToText(charCounter));




    for(TableIterator it = table.begin(); it != table.end(); ++it) {
        if (it.value() > 0)
            result << templateCountingLine
                          .arg(numberToText(it.value()))
                          .arg(it.key());
    }

    emit resultFeedback(result.join("，").append("。"));
}

bool Generator::isCharChineseLatter(QChar ch)
{
    ushort chid = ch.unicode();
    return (0x3200 < chid && chid < 0x9fff);
}

void Generator::addBuffer(QString text)
{
    int loop, len = text.length();
    if (len == 0) return;

    for (loop = 0; loop < len; ++loop) {
        QChar ch = text[loop];
        if (isCharChineseLatter(ch)) {
            buffer.append(ch);
        }
    }

}

void Generator::addString(QString text)
{
    addBuffer(text);


    while (!buffer.isEmpty()) {
        addingChar = buffer[qrand()%buffer.length()];
        addingCharCounter = buffer.count(addingChar);
        buffer.remove(addingChar);


        if (table.value(addingChar, 0) != 0) {
            deletingString += numberToText(table[addingChar]);
        }

        if (table.value(addingChar, 0) != 0) {
            deletingString.append(numberToText(table[addingChar]));
        } else {

            ++table[unitChar];
            ++addingCharCounter;
        }
        if (!deletingString.isEmpty()) {
            delString();
        }

        charCounter += addingCharCounter;
        table[addingChar] += addingCharCounter;
        addBuffer(numberToText(table[addingChar]));
        qDebug(buffer.toUtf8());
    }
}
void Generator::delString()
{
    while (!deletingString.isEmpty())
    {
        QChar deletingChar = deletingString[qrand()%deletingString.length()];
        int deletingCharCounter = deletingString.count(deletingChar);
        deletingString.remove(deletingChar);

        if (deletingCharCounter > 0 && buffer.count(deletingChar) > 0) {
            int deletingCharBuffer = buffer.count(deletingChar);
            buffer.remove(deletingChar);
            int delta = qMin(deletingCharBuffer, deletingCharCounter);

            deletingCharBuffer -= delta;
            deletingCharCounter -= delta;

            if (table.value(deletingChar, 0) != 0) {
                deletingString.append(numberToText(table[deletingChar]));
            } else {
                addBuffer(deletingChar);
            }
            table[deletingChar] += deletingCharBuffer;
            if (table.value(deletingChar, 0) != 0) {
                addBuffer(numberToText(table[deletingChar]));
            }
        }
        if (deletingCharCounter > 0 && table.value(deletingChar, 0) > 0) {
            int delta = qMin(table[deletingChar], deletingCharCounter);
            deletingString.append(numberToText(table[deletingChar]));

            table[deletingChar] -= delta;
            deletingCharCounter -= delta;

            if (table.value(deletingChar, 0) != 0) {
                addBuffer(numberToText(table[deletingChar]));
            } else {
                deletingString.append(unitChar);
                deletingString.append(deletingChar);
            }
        }
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

Generator::Generator() : QObject()
{
        templateLeadingLines.append("在这句话中");
        templateLeadingLines.append("本句中");
        templateLeadingLines.append("在此句中");

        templateLines.append("共有%1个汉字");
        templateLines.append("总计有%1个汉字");
        templateLines.append("一共有%1个汉字");

        templateCountingLine="%1个“%2”，";
        templateDetails="其中：";
        unitChar = QString("个")[0];
}

