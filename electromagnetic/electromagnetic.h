#ifndef ELECTROMAGNETIC_H
#define ELECTROMAGNETIC_H
#include <QObject>

class Electromagnetic: public QObject
{
    Q_OBJECT

public:
    explicit Electromagnetic(QObject *parent = nullptr);

public slots:
    void Electromagnetic_Power_on();  //电磁离合器上电
    void Electromagnetic_Outage();  //电磁离合器掉电
};

#endif // ELECTROMAGNETIC_H
