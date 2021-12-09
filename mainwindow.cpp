#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    reqTimer = new QTimer;
    reqTimer->setInterval(1000);

    QObject::connect(reqTimer, &QTimer::timeout, this, &MainWindow::reqTimer_timeout);
    hp3478a = new HP3478A;
    QObject::connect(hp3478a, &HP3478A::connected, this, &MainWindow::socket_connected);
    QObject::connect(hp3478a, &HP3478A::disconnected, this, &MainWindow::socket_disconnected);
    QObject::connect(hp3478a, &HP3478A::new_value, this, &MainWindow::new_value);
    socket_disconnected();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnConnect_clicked()
{
    if (!connected) {
        hp3478a->init(ui->gpibAddVolt->value(), QHostAddress(ui->leIp->text()));
    } else {
        hp3478a->deinit();
    }
}

void MainWindow::socket_connected()
{
    connected = true;
    ui->btnConnect->setText("Disconnect");
    reqTimer->start();
    ui->gpibAddVolt->setEnabled(false);
    ui->gpibAddAmp->setEnabled(false);
    ui->leIp->setEnabled(false);
}

void MainWindow::socket_disconnected()
{
    connected = false;
    ui->btnConnect->setText("Connect");
    reqTimer->stop();
    ui->gpibAddVolt->setEnabled(true);
    ui->gpibAddAmp->setEnabled(true);
    ui->leIp->setEnabled(true);
}

void MainWindow::new_value(float val)
{
    ui->valueVolt->setText(QString::number(val) + QStringLiteral(" V"));
}

void MainWindow::reqTimer_timeout()
{
    if (!hp3478a) {
        return;
    }
    hp3478a->get_value();

    QString oldText = ui->valueVolt->text();
    oldText.append(".");
    ui->valueVolt->setText(oldText);
}


