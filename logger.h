#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    void init(QDir path);
    void start();
    void stop();
    void update_data(float voltage, float current, double energy);
    void set_path(QDir path);

private:
    QDir path;
    QTimer *logTimer = nullptr;
    void log_to_file();
    QFile *file = nullptr;
    void setup_file();
    void write_header();
    float voltage;
    float current;
    float energy;

signals:
    void file_written();
};

#endif // LOGGER_H
