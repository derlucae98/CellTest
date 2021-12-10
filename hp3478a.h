#ifndef HP3478A_H
#define HP3478A_H

#include <QObject>
#include <prologixgpib.h>
#include <QStateMachine>

class HP3478A : public QObject
{
    Q_OBJECT
public:
    enum function_t {
        DCV,
        ACV,
        TWO_WIRE_OHMS,
        FOUR_WIRE_OHMS,
        DCA,
        ACA
    };
    explicit HP3478A(QObject *parent = nullptr);
    void init(quint16 addressVolt, quint16 addressAmp, QHostAddress ip);
    void deinit();
    void get_value();
    bool is_out_of_range();

private:
    PrologixGPIB *gpib = nullptr;
    quint16 addressVolt;
    quint16 addressAmp;
    void convert_value(QString input);
    bool check_value_in_range(QString &input);
    bool outOfRange;
    quint16 currentRequest;
    QStateMachine *reqMachine = nullptr;
signals:
    void connected();
    void disconnected();
    void stateChanged();
    void new_voltage(float);
    void new_current(float);


};

#endif // HP3478A_H
