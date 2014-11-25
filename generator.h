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
    void addBuffer(QString);
    void addString(QString);
    void delString();
    QString numberToText(int);
    inline bool isCharChineseLatter(QChar);

    int charCounter;
    QString buffer;
    QChar addingChar;
    int addingCharCounter;
    QString deletingString;

    typedef QMap<QChar,int>::iterator TableIterator;
    QMap<QChar,int> table;

    QChar unitChar;
    QStringList templateLeadingLines;
    QStringList templateLines;
    QString templateCountingLine;
    QString templateDetails;

};


#endif // GENERATOR_H
