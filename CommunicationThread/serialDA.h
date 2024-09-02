#ifndef SERIALDA_H
#define SERIALDA_H

#include <QWidget>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <bitset>
#include <QTime>
#include <QTimer>

class SerialDA : public QThread
{
    Q_OBJECT
public:
    explicit SerialDA(QWidget *parent = nullptr);

    void run();
    void setVol_channel(int channel, double vol);
    void setCur_channel(int channel, double Cur);
signals:

private slots:
    quint16 ModbusCRC16(QByteArray senddata);
    void ReceiveData();

private:
    QSerialPort *m_serial;


public slots:
};

#endif // SERIALTHREIO_H
