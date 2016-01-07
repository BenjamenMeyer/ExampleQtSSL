#ifndef QT_SSL_EXAMPLE_CLIENT_CONNECTION_MANAGER_H__
#define QT_SSL_EXAMPLE_CLIENT_CONNECTION_MANAGER_H__

#include <QObject>
#include <QString>
#include <QtNetwork/QHostAddress>

#include <network/tcpSocket.h>

class manager : public QObject
	{
	Q_OBJECT
	public:
		manager(QObject* _parent=NULL);
		virtual ~manager();

	public Q_SLOTS:
		void start_control();
		void stop_control();

		void add_data();
		void remove_data();

	Q_SIGNALS:
		void logMessage(QString _message);

	protected:
		QHostAddress address;
		qint16 port;

		tcpSocket control;
		listTcpSocket data;
	};

#endif //QT_SSL_EXAMPLE_CLIENT_CONNECTION_MANAGER_H__
