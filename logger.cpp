#include "logger.h"

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::init(QDir path)
{
    logTimer = new QTimer();
    logTimer->setInterval(1000); //Log every second
    QObject::connect(logTimer, &QTimer::timeout, this, &Logger::log_to_file);
    this->path = path;
    voltage = 0.0f;
    current = 0.0f;
    energy = 0.0;
}

void Logger::start()
{
    qDebug() << "Starting logger: " << path;
    setup_file();
    write_header();

    if (logTimer) {
        logTimer->start();
    }
}

void Logger::stop()
{
    if (logTimer) {
        logTimer->stop();
    }

    file->close();
}

void Logger::update_data(float voltage, float current, double energy)
{
    this->voltage = voltage;
    this->current = current;
    this->energy = energy;
}

void Logger::set_path(QDir path)
{
    this->path = path;
}

void Logger::log_to_file()
{
    if (!file->isOpen()) {
        qDebug() << "Logger: Cannot write file! File not opened.";
        logTimer->stop();
    }

    //concatenate all values to one long QString
    QString line =
            QString::number(QDateTime::currentDateTime().date().year()) + "-" +
            QString::number(QDateTime::currentDateTime().date().month()) + "-" +
            QString::number(QDateTime::currentDateTime().date().day()) + " " +
            QDateTime::currentDateTime().time().toString()  + ";" +
            QString::number(voltage)                         + ";" +
            QString::number(current)    + ";" +
            QString::number(energy);

    QTextStream out(file);
    out << line << "\r\n";
    file->flush();
    emit file_written();
}

void Logger::setup_file()
{
    QString filename =
            /* File name has format: yyyymmdd_hhmm_logfile.csv */
            /* Year */
            QString::number(QDateTime::currentDateTime().date().year()) +
            /* Month with leading 0 */
            QString("%1").arg(QDateTime::currentDateTime().date().month(), 2, 10, QChar('0')) +
            /* Day with leading 0 */
            QString("%1").arg(QDateTime::currentDateTime().date().day(), 2, 10, QChar('0')) +
            /* Underscore and hour with leading 0 */
            "_" + QString("%1").arg(QDateTime::currentDateTime().time().hour(), 2, 10, QChar('0')) +
            /* Minute with leading 0 */
            QString("%1").arg(QDateTime::currentDateTime().time().minute(), 2, 10, QChar('0')) +
            /* Underscore and filename */
            "_" + "logfile.csv";
    file = new QFile(path.absolutePath() + "/" + filename);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "Cannot create logfile!";
    }
}

void Logger::write_header()
{
    if (!file->isOpen()) {
        qDebug() << "Logger: Cannot write file! File not opened.";
        logTimer->stop();
    }

    //Value headings
    QString line =
            QStringLiteral("Timestamp")    + ";" +
            QStringLiteral("Voltage [V]")    + ";" +
            QStringLiteral("Current [A]")     + ";" +
            QStringLiteral("Energy [Ah]");

    QTextStream out(file);
    out << line << "\r\n";
}
