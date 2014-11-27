#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QChar>
#include <QSet>


class Generator : public QObject
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
    QString numberToText(int);
    inline bool isCharChineseLatter(QChar);

    int totalCharCounter;

    typedef QMap<QChar,int>::iterator CharIntMapIterator;
    QMap<QChar,int> resultMap, bufferMap;
    typedef QSet<QChar> CharSet;

    QChar unitChar;
    QStringList templateLeadingLines;
    QStringList templateLines;
    QString templateCountingLine;
    QString templateDetails;

    QString negitiveChar;
    QString numberChars;
    int unitCharCount;
    QString unitChars[4];
    QString groupUnitChar[4];
    QString specialLoopChars;
};


#endif // GENERATOR_H
