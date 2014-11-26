#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QChar>


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
    void delString(QString);
    QString numberToText(int);
    inline bool isCharChineseLatter(QChar);
    void numberChanged(int from, int to);

    int totalCharCounter;

    typedef QMap<QChar,int>::iterator QMapChIntIterator;
    QMap<QChar,int> resultMap, workingMap, bufferMap;

    QChar unitChar;
    QStringList templateLeadingLines;
    QStringList templateLines;
    QString templateCountingLine;
    QString templateDetails;
};


#endif // GENERATOR_H
