#ifndef GENERATER_H
#define GENERATER_H

#include <QObject>

class generater : public QObject
{
    Q_OBJECT
public:
    explicit generater(QObject *parent = 0);

signals:
    void generated();
    void generatingPercent();
public slots:

};

#endif // GENERATER_H
