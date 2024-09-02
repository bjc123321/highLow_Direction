#ifndef SERIALAD_H
#define SERIALAD_H

#include <QWidget>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <bitset>
#include <QTime>
#include <QTimer>

class SerialAD : public QThread
{
    Q_OBJECT
public:
    explicit SerialAD(QWidget *parent = nullptr);

    void run();

signals:
    void sendValue(QList<double> &value_list);

private slots:
    void ReceiveData();
    quint16 ModbusCRC16(QByteArray senddata);
    void ADSendStr(QString str);
    void AD_16GetValue();
    void com_16BatAnalyze(QByteArray &allData);

    void timer1Slot();
private:
    QSerialPort *m_serial;
    QByteArray sdData;
    QTimer timer1;

public slots:
};

#endif // SERIALTHREAD_H
