#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QChar>
#include <QSet>

class Generator : public QThread
{
    Q_OBJECT

public:
    Generator();

public slots:
    void generate(QString leadingText);

signals:
    void resultFeedback(QString);

private:
    void addString(QString);
    void delString(QString);
    QString numberToText(int);
    inline bool isCharChineseLatter(QChar);
    bool syncValue(int from, int to);

    typedef QSet<QChar> CharSet;

    class CharCounter
    {
    public:
        typedef QMap<QChar,int>::iterator iterator;
        QMap<QChar,int> map;
        int total;

        CharCounter() {clear();}
        inline void clear()
        {
            map.clear();
            total = 0;
        }

        inline int value(QChar ch)
        {
            return map.value(ch, 0);
        }

        inline int addChar(QChar ch)
        {
            map[ch] = map.value(ch, 0) + 1;
            ++total;
            return map.value(ch, 0);
        }

        inline int delChar(QChar ch)
        {
            map[ch] = map.value(ch, 0) - 1;
            --total;
            if (map.value(ch, 0) == 0) {
                map.remove(ch);
            }
            return map.value(ch, 0);
        }

        inline bool operator == (CharCounter &other)
        {
            if (total != other.total) return false;
            return (map == other.map);
        }

        inline bool operator != (CharCounter &other)
        {
            return !(*this == other);
        }
    };

    CharCounter resultCounter, prevCounter, oldCounter;
    inline QString resultText(CharCounter& cnt);

    QChar unitChar;
    QString templateLeadingLine;
    QString templateAppendingLine;
    QString templateCountingLine;

    QString negitiveChar;
    QString numberChars;
    int unitCharCount;
    QString unitChars[4];
    QString groupUnitChar[4];
    QString seedingText;
};

#endif // GENERATOR_H
