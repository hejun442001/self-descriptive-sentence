#include "generator.h"

Generator::Generator() : QThread()
{
    templateLeadingLine = QString("这句话是自计数句，句中");
    templateAppendingLine = QString("共有%1个字");

    templateCountingLine="，有%1个“%2”";
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
    qDebug(qUtf8Printable("SEEDING:" + text));
    addString(seedingText.arg(""));

    int limit = 1;
    int MaxLoop = text.length();
    for ( ; limit <= MaxLoop && prevCounter != resultCounter; ++limit) {

        oldCounter = CharCounter(prevCounter);
        prevCounter = CharCounter(resultCounter);

        MaxLoop = resultCounter.total * 10;
        QString feedback = QString("%1/%2:").arg(limit).arg(MaxLoop);


        if (!syncValue(oldCounter.total, prevCounter.total,
                       resultCounter.total)) {

            feedback +=QString("TC(%1->%2:%3)")
                    .arg(oldCounter.total)
                    .arg(prevCounter.total)
                    .arg(resultCounter.total);
        } else {
            feedback += QString("(TC:%1)")
                    .arg(resultCounter.total);
        }

        CharSet keys = oldCounter.map.keys().toSet() +
                prevCounter.map.keys().toSet();
        for (CharSet::iterator itor = keys.begin();
             itor != keys.end(); ++itor) {
            QChar key = *itor;
            int oldValue = oldCounter.value(key);
            int value = prevCounter.value(*itor);
            int &monitor = resultCounter.map[key];

            if (oldValue < 0 || value < 0 || monitor < 0) {
                feedback += QString(",ERROR:%1(%2->%3:%4)")
                        .arg(key).arg(oldValue)
                        .arg(value).arg(monitor);
                Q_ASSERT_X (false, "Generator::generate()", qUtf8Printable(feedback));

                feedback.prepend("DISCARDED:");
                oldCounter.clear();
                prevCounter.clear();
                resultCounter.clear();
                addString(seedingText.arg(""));
                break;
            }


            if (!syncValue(oldValue, value, monitor)) {
                if (oldValue == 0 && value > 0) {
                    QString tmp = templateCountingLine
                            .arg("").arg(key);
                    addString(tmp);
                }

                feedback += QString(",%1(%2->%3:%4)")
                        .arg(key).arg(oldValue)
                        .arg(value).arg(monitor);
            } else {
                feedback += QString(",%1=%2").arg(key).arg(monitor);
            }
        }

        qDebug(qUtf8Printable(feedback));
    }
    limit--;

    QString feedback = resultText(resultCounter) + "\n";
    qDebug(qUtf8Printable("RESULT:"+feedback));
    if (oldCounter != prevCounter || prevCounter != resultCounter) {
        QString prepend = QString("      抱歉！\n");
        prepend += QString("在%1次迭代后发现无法基于以下模板生成自计数句")
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
            feedback += resultText(prevCounter) + "\n";
        }
        if (limit > 2) {
            feedback += timesLine.arg(numberToText(limit - 2));
            feedback += resultText(oldCounter) + "\n";
        }
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
            resultCounter.addChar(ch);
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
            resultCounter.delChar(ch);
        }
    }
}


bool Generator::syncValue(const int from, const int to, int &monitor) {
    if (from != to) {
        if (from > 0) {
            delString(numberToText(from));
        }

        if (to > 0) {
            addString(numberToText(to));
        }
        return false;
    }
    return (to == monitor);
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
                    .arg(numberToText(itor.value()))
                    .arg(itor.key());
    }
    return result.append("。");
}
