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

void HP3478A::init(quint16 addressVolt, quint16 addressAmp, QHostAddress ip)
{
    if (!gpib) {
        return;
    }
    this->addressVolt = addressVolt;
    this->addressAmp = addressAmp;
    currentRequest = this->addressVolt;
    gpib->init(ip);
//    reqMachine = new QStateMachine;
//    QState *wait = new QState;
//    QState *reqVolt = new QState;
//    QState *convVolt = new QState;
//    QState *reqAmp = new QState;
//    QState *convAmp = new QState;


//    wait->addTransition(this, &HP3478A::startConv, reqVolt);
//    reqVolt->addTransition(this, &HP3478A::respVolt, convVolt);
//    convVolt->addTransition(this, &HP3478A::convFinished, reqAmp);
//    reqAmp->addTransition(this, &HP3478A::respAmp, convAmp);
//    convAmp->addTransition(this, &HP3478A::convFinished, wait);

//    QObject::connect(reqVolt, &QAbstractState::entered, this, [=] {get_value(this->addressVolt);});
//    QObject::connect(convVolt, &QAbstractState)

}

void HP3478A::deinit()
{
    if (!gpib) {
        return;
    }
    gpib->deinit();
}


void HP3478A::get_value()
{
    if (!gpib) {
        return;
    }
    qDebug() << "Requesting DMM at address " << currentRequest;
    if (currentRequest == this->addressVolt) {
        gpib->send_command(addressVolt, "++read eoi");
    } else if (currentRequest == this->addressAmp) {
        gpib->send_command(addressAmp, "++read eoi");
    }
}

bool HP3478A::is_out_of_range()
{

}

void HP3478A::convert_value(QString input)
{

    float value;
    if (input.at(0) == '+' || input.at(0) == '-') {
        //Check if the returned value is in the format of a value response
        value = input.toFloat();
        qDebug() << "Got valid response from DMM at address " << currentRequest;
        if (currentRequest == this->addressVolt) {
            emit new_voltage(value);
            currentRequest = this->addressAmp;
            get_value();
        } else if (currentRequest == this->addressAmp) {
            emit new_current(value);
            currentRequest = this->addressVolt;
        }
    }
}

bool HP3478A::check_value_in_range(QString &input)
{

}
