#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QTimer>
#include <hp3478a.h>
#include <QDir>
#include <QFileDialog>
#include <logger.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();


    void on_btnSelectPath_clicked();

private:
    Ui::MainWindow *ui;
    HP3478A *hp3478a = nullptr;
    QTimer *reqTimer = nullptr;
    void reqTimer_timeout();
    void socket_connected();
    void socket_disconnected();
    bool connected;
    void new_voltage(float val);
    void new_current(float val);
    void update_logfile_path(QDir newPath);
    QDir logfilePath;
    float voltage;
    float current;
    double energy;
    Logger *logger = nullptr;
};
#endif // MAINWINDOW_H
