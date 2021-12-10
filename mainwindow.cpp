#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    reqTimer = new QTimer;
    reqTimer->setTimerType(Qt::PreciseTimer);
    reqTimer->setInterval(1000);

    QObject::connect(reqTimer, &QTimer::timeout, this, &MainWindow::reqTimer_timeout);
    hp3478a = new HP3478A;
    QObject::connect(hp3478a, &HP3478A::connected, this, &MainWindow::socket_connected);
    QObject::connect(hp3478a, &HP3478A::disconnected, this, &MainWindow::socket_disconnected);
    QObject::connect(hp3478a, &HP3478A::new_voltage, this, &MainWindow::new_voltage);
    QObject::connect(hp3478a, &HP3478A::new_current, this, &MainWindow::new_current);
    socket_disconnected();

    logger = new Logger;
    QObject::connect(logger, &Logger::file_written, this, [=] {ui->statusbar->showMessage("File written!", 500);});
    logger->init(this->logfilePath);

    update_logfile_path(QDir::home());

    voltage = 0.0f;
    current = 0.0f;
    energy = 0.0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    if (!connected) {
        hp3478a->init(ui->gpibAddVolt->value(), ui->gpibAddAmp->value(), QHostAddress(ui->leIp->text()));
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
    ui->btnSelectPath->setEnabled(false);
    if (logger) {
        logger->start();
    }
}

void MainWindow::socket_disconnected()
{
    connected = false;
    ui->btnConnect->setText("Connect");
    reqTimer->stop();
    ui->gpibAddVolt->setEnabled(true);
    ui->gpibAddAmp->setEnabled(true);
    ui->leIp->setEnabled(true);
    ui->btnSelectPath->setEnabled(true);
    if (logger) {
        logger->stop();
    }
}

void MainWindow::new_current(float val)
{
    ui->valueAmp->setText(QString::number(val) + QStringLiteral(" A"));
    current = val;
}

void MainWindow::new_voltage(float val)
{
    ui->valueVolt->setText(QString::number(val) + QStringLiteral(" V"));
    voltage = val;
}

void MainWindow::update_logfile_path(QDir newPath)
{
    ui->logfilePath->setText(newPath.absolutePath());
    ui->logfilePath->setToolTip(newPath.absolutePath());
    this->logfilePath = newPath;
    if (logger) {
        logger->set_path(newPath);
    }
}

void MainWindow::reqTimer_timeout()
{
    if (!hp3478a) {
        return;
    }
    hp3478a->get_value();

    //Flashing dot: DMM responds with new value
    //Dot appears permanentely: DMM does not respond
    QString oldText = ui->valueVolt->text();
    if (oldText.at(oldText.length()-1) != '.') {
        oldText.append('.');
    }
    ui->valueVolt->setText(oldText);

    oldText = ui->valueAmp->text();
    if (oldText.at(oldText.length()-1) != '.') {
        oldText.append('.');
    }
    ui->valueAmp->setText(oldText);

    if (current > 0.001f) {
        ui->lStatus->setText("Charging...");
    } else if (current < -0.001f) {
        ui->lStatus->setText("Discharging...");
    } else {
        ui->lStatus->setText("Idle.");
    }

    energy += current / 3600.0;
    ui->lEnergy->setText(QString::number(energy) + " Ah");
    if (logger) {
        logger->update_data(voltage, current, energy);
    }
}

void MainWindow::on_btnSelectPath_clicked()
{
    QFileDialog pathSelectDialog;
    pathSelectDialog.setFileMode(QFileDialog::Directory);
    if (pathSelectDialog.exec()){
        update_logfile_path(pathSelectDialog.directory().absolutePath());
    }
}

