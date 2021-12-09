#ifndef HP3478A_H
#define HP3478A_H

#include <QObject>
#include <prologixgpib.h>

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
    void init(quint16 gpibAddress, QHostAddress ip);
    void deinit();
    void set_function(function_t function);
    void get_value();
    bool is_out_of_range();

private:
    PrologixGPIB *gpib = nullptr;
    quint16 gpibAddress;
    void convert_value(QString input);
    bool check_value_in_range(QString &input);
    bool outOfRange;
signals:
    void connected();
    void disconnected();
    void stateChanged();
    void new_value(float);

};

#endif // HP3478A_H
