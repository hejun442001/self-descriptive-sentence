#include "generator.h"

void Generator::generate(QString text)
{
    QStringList result;
    int leadingTextSelector= qrand() % templateLeadingLines.size();
    int appendTextSelector = qrand() % templateLines.size();
    int oldTotalCharCounter;
    resultMap.clear();
    QString resultText;
    QString leadingText = templateLeadingLines[leadingTextSelector];
    QString appendText = templateLines[appendTextSelector];

    bool numberChanged;
    int loopTimes = 0;

    bufferMap.clear();
    totalCharCounter = 0;

    if (text.contains(QString("。"))
            || text.contains(QString("？"))
            || text.contains(QString("！"))) {
        appendText.replace(QString("句"), QString("段"));
    } else if (text.endsWith(QString("；"))
               || text.endsWith(QString("，"))
               || text.endsWith(QString("。"))
               || text.endsWith(QString("？"))
               || text.endsWith(QString("！"))
               || text.endsWith(QString("”"))) {
        ; // do nothing
    } else if (text.contains(QString("；"))) {
        text.append("；");
    } else {
        text.append("，");
    }

    result << leadingText + appendText << templateDetails;
    text += result.join(QString("，"));
    addString(text);
    totalCharCounter += bufferMap.size() * 3;
    addString(numberToText(totalCharCounter));

    do {
        resultMap = bufferMap;
        bufferMap.clear();

        resultText = text.arg(numberToText(totalCharCounter));
        result.clear();
        for(CharIntMapIterator it = resultMap.begin();
            it != resultMap.end(); ++it) {
            result << templateCountingLine
                      .arg(numberToText(it.value()))
                      .arg(it.key());
        }
        resultText += result.join("，").append("。");

        oldTotalCharCounter = totalCharCounter;
        totalCharCounter = 0;

        addString(resultText);


        if (++loopTimes > totalCharCounter) {
            QString feedback = QString("%1次迭代后可能不正确的结果")
                    .arg(numberToText(loopTimes));
            feedback += QString("(此仅用于展示自描述句的格式)：\n\n")
                    .append(resultText);;
            qDebug(qUtf8Printable("!!! abnormal end !!!\n\n"));
            emit resultFeedback(feedback);
            return;
        }
        qDebug(qUtf8Printable(QString("%1").arg(loopTimes)+":"+resultText));
        numberChanged = false;
        if (oldTotalCharCounter == totalCharCounter) {
            CharSet resultMapKeys = CharSet::fromList(resultMap.keys());
            CharSet bufferMapKeys = CharSet::fromList(bufferMap.keys());
            CharSet totalMapKeys = resultMapKeys + bufferMapKeys;
            for (CharSet::iterator itor = totalMapKeys.begin();
                 itor != totalMapKeys.end(); ++itor) {
                if (resultMap.value(*itor, 0) != bufferMap.value(*itor, 0)) {
                    numberChanged = true;
                    break;
                }
            }
        }

    } while (oldTotalCharCounter != totalCharCounter || numberChanged);



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
            bufferMap[ch] = value + 1;
            ++totalCharCounter;
        }
    }
}

QString Generator::numberToText(int n)
{
    QString result;

    if (n == 0) {
        return QString(numberChars[0]);
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
                result.append(numberChars[0]);
            }

            if (digit != 1 || unit != 1 || ! zeroGroup
                    || (group == 0 && zeroNeed)) {
                result.append(numberChars[digit]);
            }

            result.append(unitChars[unit]);
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

Generator::Generator() : QObject()
{
    templateLeadingLines.append("在这句话中");
    templateLeadingLines.append("在此句中");
    templateLeadingLines.append("在本句中");
    templateLeadingLines.append("这句话中");
    templateLeadingLines.append("此句中");
    templateLeadingLines.append("本句中");
    templateLeadingLines.append("句中");

    templateLines.append("有%1个字");
    templateLines.append("共有%1个字");
    templateLines.append("总共有%1个字");
    templateLines.append("共计%1个字");
    templateLines.append("一共有%1个字");

    templateCountingLine="%1个“%2”";
    templateDetails="其中：";
    unitChar = QString("个")[0];


    negitiveChar = "负";
    numberChars = "零一二三四五六七八九";
    specialLoopChars = negitiveChar+numberChars;

    QString _unitChars[] = {"", "十", "百", "千"};
    unitCharCount = 4;
    QString _groupUnitChar[] = {"", "万", "亿", "兆"};
    for (int iLoop = 1; iLoop < unitCharCount; ++iLoop) {
        unitChars[iLoop] = _unitChars[iLoop];
        groupUnitChar[iLoop] = _groupUnitChar[iLoop];
        specialLoopChars += _unitChars[iLoop] + _groupUnitChar[iLoop];
    }
}

