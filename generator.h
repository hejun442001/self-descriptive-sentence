#ifndef GENERATOR_H
#define GENERATOR_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QChar>


class Generator
{
public:
    Generator();
    QString generate(QString leadingText);

private:
    void addBuffer(QString);
    QString numberToText(int);

    QString buffer;
    bool deadlocked;
    QMap<QChar,int> table;

    QStringList templateLeadingLines;
    QStringList templateLines;


};


#endif // GENERATOR_H
