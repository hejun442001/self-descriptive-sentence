#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QChar>


class Generator : public QThread
{
    Q_OBJECT

public:
    Generator();

public slots:
    void generate(QString leadingText);

private:
    void addBuffer(QString);
    QString numberToText(int);
    inline bool isCharChineseLatter(QChar);

    QString buffer;
    bool deadlocked;
    QMap<QChar,int> table;
    int charCounter;

    QStringList templateLeadingLines;
    QStringList templateLines;

};


#endif // GENERATOR_H
