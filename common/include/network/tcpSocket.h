#ifndef QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_H__
#define QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_H__

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslCertificate>
#include <QPointer>
#include <QThread>
#include <QList>
#include <cstdint>

#include <network/tcpSocketManager.h>

// Purpose: to read/write to sockets in dedicated threads
class tcpSocket : public QObject
	{
	Q_OBJECT
	public:
		tcpSocket(QObject* _parent=NULL, bool is_server_socket=false);
		virtual ~tcpSocket();

		void create(QHostAddress _address, qint16 _port);
		void destroy();

		void setSocketDescriptor(qintptr socketDescriptor);
		QSharedPointer<QSslSocket> getSocket() const;

		// required for SSL Server
		const QSslKey& privateKey() const;
		void setPrivateKey(const QSslKey& key);

		// required for SSL Server
		const QSslCertificate& localCertificate() const;
		void setLocalCertificate(const QSslCertificate& certificate);

		bool isServerSocket() const;

	Q_SIGNALS:
		void close();
		void sendMessage(QByteArray _data);
		void sendJson(QByteArray _jsonDocument);
		void receivedMessage(QByteArray _data);
		void receiveJson(QByteArray _jsonDocument);
		void logMessage(QString _message);

	private:
		// thread for the socket
		QThread thread;

		// object for read/writing to the thread
		tcpSocketManager socketManager;
		bool server_side_socket;

		// For Encryption support
		QSslKey sslKey;
		QSslCertificate sslCertificate;

	private Q_SLOTS:
		void encrypted();
		void onSocketError(QAbstractSocket::SocketError _error);
		void onSslErrors(const QList<QSslError>& errors);
	};

typedef QSharedPointer<tcpSocket> ptrTcpSocket;
typedef QList<ptrTcpSocket> listTcpSocket;

#endif //QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_H__
