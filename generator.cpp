#include "generator.h"

Generator::Generator() : QThread()
{
    templateLeadingLine = QString("在这句话中");

    templateAppendingLine = QString("有%1个字");

    templateCountingLine="，%1个“%2”";
    unitChar = QString("个")[0];

    negitiveChar = "负";
    numberChars = "零一二三四五六七八九";
    unitCharCount = 4;

    QString _unitChars[] = {"", "十", "百", "千"};
    QString _groupUnitChar[] = {"", "万", "亿", "兆"};
    for (int iLoop = 1; iLoop < unitCharCount; ++iLoop) {
        unitChars[iLoop] = _unitChars[iLoop];
        groupUnitChar[iLoop] = _groupUnitChar[iLoop];
    }
}

void Generator::generate(QString text)
{
    if (text.isEmpty()) {
        seedingText = templateLeadingLine + templateAppendingLine;
    } else {
        seedingText = text + templateAppendingLine;
    }

    resultCounter.clear();
    prevCounter.clear();
    oldCounter.clear();

    text = seedingText.arg("____");
    qDebug(qUtf8Printable(" SEEDING :" + text));
    addString(text);

    int limit=0;
    int MaxLoop = resultCounter.total;
    for ( ; limit <= MaxLoop && resultCounter != prevCounter; ++limit) {
        MaxLoop = qMax(MaxLoop, resultCounter.total);

        oldCounter = prevCounter;
        prevCounter = resultCounter;

        qDebug(qUtf8Printable(QString("%1| START |%2")
                              .arg(limit + 1)
                              .arg(resultText(resultCounter))));

        CharSet keys = oldCounter.map.keys().toSet()
                + prevCounter.map.keys().toSet();
        for (CharSet::iterator itor = keys.begin();
             itor != keys.end(); ++itor) {

            if (syncValue(oldCounter.value(*itor),
                      prevCounter.value(*itor))) {
                qDebug(qUtf8Printable(QString("%1|%2:%3->%4=%5|%6")
                                      .arg(limit + 1).arg(*itor)
                                      .arg(oldCounter.value(*itor))
                                      .arg(prevCounter.value(*itor))
                                      .arg(resultCounter.value(*itor))
                                      .arg(resultText(resultCounter))));

            }
        }

        if (syncValue(oldCounter.total, prevCounter.total)) {
            qDebug(qUtf8Printable(QString("%1|TC:%2->%3=%4|%6")
                                  .arg(limit + 1)
                                  .arg(oldCounter.total)
                                  .arg(prevCounter.total)
                                  .arg(resultCounter.total)
                                  .arg(resultText(resultCounter))));
        }

    }

    QString feedback = resultText(resultCounter);
    if (resultCounter != prevCounter) {
        QString prepend = QString("      抱歉！\n");
        prepend += QString("在%1次迭代后发现无法基于以下模板生成自描述句")
                        .arg(numberToText(limit));
        prepend += QString("(请变更“句首部分”文本后再试)：\n");
        prepend += text + "\n\n";
        prepend += QString("以下是最后%1次迭代结果：\n")
                        .arg(numberChars[qMin(3, limit)]);
        QString timesLine = QString("第%1次：\n");
        prepend += timesLine.arg(numberToText(limit));
        feedback.prepend(prepend);
        if (limit > 1) {
            feedback += timesLine.arg(numberToText(limit - 1));
            feedback += resultText(prevCounter);
        }
        if (limit > 2) {
            feedback += timesLine.arg(numberToText(limit - 2));
            feedback += resultText(oldCounter);
        }
        feedback += QString("\n：\n");
        qDebug(qUtf8Printable("!!! failed !!!\n\n"));
    } else {
        qDebug(qUtf8Printable("!!! ok !!!\n\n"));
    }
    emit resultFeedback(feedback);
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
            if (resultCounter.addChar(ch) == 1) {
                resultCounter.addChar(ch);
                resultCounter.addChar(unitChar);
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
            if (resultCounter.delChar(ch) == 1) {
//                resultCounter.delChar(ch);
//                resultCounter.delChar(unitChar);
            }
        }
    }
}


bool Generator::syncValue(int from, int to) {
    if (from != to) {

       if (from != 0) {
           delString(numberToText(from));
       }
       if (to != 0) {
           addString(numberToText(to));
       }
       return true;
    }
    return false;
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

QString Generator::resultText(CharCounter &cnt)
{
    QString result = seedingText.arg(numberToText(cnt.total));
    for (CharCounter::iterator itor = cnt.map.begin();
         itor != cnt.map.end(); ++itor) {
        result += templateCountingLine
                  .arg(numberToText(itor.value())).arg(itor.key());
    }
    return result.append("。\n");
}
