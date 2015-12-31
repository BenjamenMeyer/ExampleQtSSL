#include <log/logger.h>

#include <QDateTime>
#include <QString>

using namespace exampleQtSsl::common::logger;

logger::logger(QString _name, QObject* _parent) : QObject(_parent), name(_name)
    {
    QDateTime utcnow = QDateTime::currentDateTimeUtc();
    QString filename = QString("%1_%2.log").arg(utcnow.toString("yyyymmdd_HHmmss-zzz")).arg(name);
    logFile.setFileName(filename);
    logFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Unbuffered);
    theLog.setDevice(&logFile);
    logMessage(tr("Starting log"));
    }
logger::~logger()
    {
    logMessage(tr("Stopping log"));
    if (logFile.isOpen() == true)
        {
        logFile.close();
        }
    }

void logger::logMessage(QString _message)
    {
    if (logFile.isOpen() == true)
        {
        QDateTime utcnow = QDateTime::currentDateTimeUtc();
        QString message = QString("%1 - %2\n").arg(utcnow.toString("yyyy-mm-dd HH:mm:ss.zzz")).arg(_message);
        theLog<<message;
        }
    }
