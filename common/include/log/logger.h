#ifndef QT_SSL_EXAMPLE_COMMON_LOGGER_H__
#define QT_SSL_EXAMPLE_COMMON_LOGGER_H__

#include <QObject>
#include <QFile>
#include <QString>
#include <QPointer>
#include <QTextStream>

class logger: public QObject
	{
	Q_OBJECT
	public:
		logger(QString _name, QObject* _parent=NULL);
		virtual ~logger();

	public Q_SLOTS:
		void logMessage(QString _message);

	protected:
		QString name;
		QFile logFile;
		QTextStream theLog;
	};

#endif //QT_SSL_EXAMPLE_COMMON_LOGGER_H__
