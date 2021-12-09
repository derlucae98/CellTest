#include "hp3478a.h"

HP3478A::HP3478A(QObject *parent) : QObject(parent)
{
    gpib = new PrologixGPIB;
    if (!gpib) {
        return;
    }
    QObject::connect(gpib, &PrologixGPIB::connected, this, &HP3478A::connected); //Forwarding signal
    QObject::connect(gpib, &PrologixGPIB::disconnected, this, &HP3478A::disconnected); //Forwarding signal
    QObject::connect(gpib, &PrologixGPIB::stateChanged, this, &HP3478A::stateChanged); //Forwarding signal
    QObject::connect(gpib, &PrologixGPIB::response, this, &HP3478A::convert_value);
}

void HP3478A::init(quint16 gpibAddress, QHostAddress ip)
{
    if (!gpib) {
        return;
    }
    this->gpibAddress = gpibAddress;
    gpib->init(ip);
}

void HP3478A::deinit()
{
    if (!gpib) {
        return;
    }
    gpib->deinit();
}

void HP3478A::set_function(function_t function)
{
    if (!gpib) {
        return;
    }
    switch (function) {
    case HP3478A::DCV:
        gpib->send_command(this->gpibAddress, "F1");
        break;
    case HP3478A::ACV:
        gpib->send_command(this->gpibAddress, "F2");
        break;
    case HP3478A::TWO_WIRE_OHMS:
        gpib->send_command(this->gpibAddress, "F3");
        break;
    case HP3478A::FOUR_WIRE_OHMS:
        gpib->send_command(this->gpibAddress, "F4");
        break;
    case HP3478A::DCA:
        gpib->send_command(this->gpibAddress, "F5");
        break;
    case HP3478A::ACA:
        gpib->send_command(this->gpibAddress, "F6");
        break;
    default:
        break;
    }
}

void HP3478A::get_value()
{
    if (!gpib) {
        return;
    }
    gpib->send_command(this->gpibAddress, "++read eoi");
}

bool HP3478A::is_out_of_range()
{

}

void HP3478A::convert_value(QString input)
{
    qDebug() << input;
    float value;
    if (input.at(0) == '+' || input.at(0) == '-') {
        //Check if the returned value is in the format of a value response
        value = input.toFloat();
        qDebug() << value;
        emit new_value(value);
    }
}

bool HP3478A::check_value_in_range(QString &input)
{

}
