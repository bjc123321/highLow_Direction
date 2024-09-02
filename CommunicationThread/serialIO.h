#ifndef SERIALIO_H
#define SERIALIO_H

#include <QWidget>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <bitset>
#include <QTime>
#include <QTimer>

class SerialIO : public QThread
{
    Q_OBJECT
public:
    explicit SerialIO(QWidget *parent = nullptr);

    void run();
    void set_channel(int channel, bool flag);
signals:
    void sendDIState(QList<bool> DI_state);
private slots:
    quint16 ModbusCRC16(QByteArray senddata);
    void DISendStr();
    void timer1Slot();
    void ReceiveData();
    void comBatAnalyze(QByteArray &allData);

private:
    QSerialPort *m_serial;
    QTimer timer1;
    QByteArray comData;


public slots:
};

#endif // SERIALTHREIO_H
