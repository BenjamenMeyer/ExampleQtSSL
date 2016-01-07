#ifndef QT_SSL_EXAMPLE_SERVER_GUI_H__
#define QT_SSL_EXAMPLE_SERVER_GUI_H__

#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslCertificate>
#include <QPointer>
#include <QPushButton>
#include <QTextEdit>
#include <QThread>
#include <QWidget>

#include <service/manager.h>
#include <log/logger.h>

class userInterface: public QWidget
	{
	Q_OBJECT
	public:
		userInterface(QSslKey key, QSslCertificate certificate, QWidget* _parent=NULL, Qt::WindowFlags _flags=0);
		virtual ~userInterface();

	public Q_SLOTS:
		void startService();
		void stopService();
		void logMessage(QString _message);

	Q_SIGNALS:
		void start_service();
		void stop_service();
		void send_to_log(QString _message);

	protected:
		QPointer<QTextEdit> textDisplay;
		QPointer<QPushButton> buttonStart;
		QPointer<QPushButton> buttonStop;

		QSslKey sslKey;
		QSslCertificate sslCertificate;

		// thread that runs the actual service
		// what it contains will eventually be a thread in a daemon service
		QThread serviceThread;
		manager service;

		// thread that runs the file logger
		QThread loggingThread;
		logger log;

		void createLayout();
	};

#endif //QT_SSL_EXAMPLE_SERVER_GUI_H__
