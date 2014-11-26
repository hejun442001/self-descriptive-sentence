#include "generator.h"
const int LOOP_LIMIT = 1000;

void Generator::generate(QString text)
{
    if (!text.isEmpty()) {
        // @todo :: fix it
        text=text
                .split(QRegExp("[。！？\\.\\?\\!\\s\\n]{1,}![”]"),QString::SkipEmptyParts)
                .join("，");
    }
    int leadingTextSelector=qrand()%templateLeadingLines.size();
    int appendTextSelector = qrand() % templateLines.size();
    int oldTotalCharCounter;

    bool numberChangedFlag;
    QString resultText;

    QString leadingText = templateLeadingLines[leadingTextSelector];
    QString appendText = templateLines[appendTextSelector];
    bufferMap.clear();
    resultMap.clear();
    workingMap.clear();
    oldTotalCharCounter = 0;
    totalCharCounter = 0;
    int loopTimes = 0;

    addString(text);
    addString(leadingText);
    addString(appendText);
    addString(templateDetails);
    addString(numberToText(totalCharCounter));

    while (numberChangedFlag || oldTotalCharCounter != totalCharCounter) {
        numberChangedFlag = false;

        workingMap = bufferMap;

        for (QMapChIntIterator it = workingMap.begin();
             it != workingMap.end(); it++) {
            QChar key = it.key();
            int oldValue = resultMap.value(key, 0);
            int value = workingMap.value(key, 0);

            if (value != oldValue) {
                numberChanged(oldValue, value);
                numberChangedFlag = true;
            }
        }

        if (oldTotalCharCounter != totalCharCounter) {
            numberChanged(oldTotalCharCounter, totalCharCounter);
            oldTotalCharCounter = totalCharCounter;
            numberChangedFlag = true;
        }

        resultMap = workingMap;

        QStringList result;
        if (!text.isEmpty()) result << text;
        result << leadingText
               << appendText.arg(numberToText(totalCharCounter))
               << templateDetails;
        resultText = result.join("，");
        result.clear();
        for(QMapChIntIterator it = resultMap.begin(); it != resultMap.end(); ++it) {
            if (it.value() > 0)
                result << templateCountingLine
                          .arg(numberToText(it.value()))
                          .arg(it.key());
        }

        resultText += result.join("，").append("。");

        qDebug(qUtf8Printable(resultText));
        if (++loopTimes >= LOOP_LIMIT) {
           QString feedback = QString("%1次迭代后可能不正确的结果：\n\n")
                   .arg(LOOP_LIMIT).append(resultText);
           qDebug(qUtf8Printable("!!! abnormal end !!!\n\n"));
           emit resultFeedback(feedback);
           return;
        }
    }
    qDebug(qUtf8Printable("!!! end !!!\n\n"));
    emit resultFeedback(resultText);
}

bool Generator::isCharChineseLatter(QChar ch)
{
    ushort chid = ch.unicode();
    return (0x3200 < chid && chid < 0x9fff);
}

void Generator::addString(QString text)
{
    int loop, len = text.length();
    if (len == 0) return;

    for (loop = 0; loop < len; ++loop) {
        QChar ch = text[loop];
        if (isCharChineseLatter(ch)) {
            int value = bufferMap.value(ch, 0);
            if (value == 0) {
                bufferMap[ch] = 2;
                totalCharCounter += 2;
                addString(unitChar);
            } else {
                bufferMap[ch] = value + 1;
                ++totalCharCounter;
            }
        }
    }
}

void Generator::delString(QString text)
{
    int loop, len = text.length();
    if (len == 0) return;

    for (loop = 0; loop < len; ++loop) {
        QChar ch = text[loop];
        if (isCharChineseLatter(ch)) {
            int value = bufferMap.value(ch, 0);
            if (value == 2) {
                bufferMap.remove(ch);
                totalCharCounter -= 2;
                delString(unitChar);
            } else {
                bufferMap[ch] = value - 1;
                totalCharCounter -= 1;
            }
        }
    }
}

QString Generator::numberToText(int n)
{
    const QString numberChar = "零一二三四五六七八九";
    const QString unitChar[] = {"", "十", "百", "千"};
    const int unitCharCount = 4;
    const QString groupUnitChar[] = {"", "万", "亿", "兆"};
    QString result;

    if (n == 0) {
        return numberChar[0];
    }
    if (n < 0) {
        result = "负";
        n = -n;
    }

    QMap<int, int> digits;
    int bit = 0;
    while (n) {
        digits[bit] = n % 10;
        n /= 10;
        ++bit;
    }

    bool zeroNeed = false;
    bool zeroGroup = true;

    for (--bit ; bit >= 0; --bit) {
        int digit = digits[bit];
        int unit = bit % unitCharCount;
        int group = bit / unitCharCount;

        if (digit != 0) {
            if (zeroNeed) {
                result.append(numberChar[0]);
            }

            if (digit != 1 || unit != 1 || ! zeroGroup
                    || (group == 0 && zeroNeed)) {
                result.append(numberChar[digit]);
            }

            result.append(unitChar[unit]);
        }

        zeroGroup = zeroGroup && (digit ==0);

        if (unit == 0 && !zeroGroup) {
            result.append(groupUnitChar[group]);
        }

        zeroNeed = (digit == 0 && (unit != 0 || zeroGroup));
        zeroGroup = zeroGroup || (unit == 0);
    }


    return result;
}

void Generator::numberChanged(int from, int to)
{
    if (from == to) return;
    if (from != 0) delString(numberToText(from));
    if (to != 0) addString(numberToText(to));
}

Generator::Generator() : QObject()
{
    templateLeadingLines.append("在这句话中");
    templateLeadingLines.append("本句中");
    templateLeadingLines.append("在此句中");

    templateLines.append("共有%1个汉字");
    templateLines.append("总计有%1个汉字");
    templateLines.append("一共有%1个汉字");

    templateCountingLine="%1个“%2”";
    templateDetails="其中：";
    unitChar = QString("个")[0];
}

