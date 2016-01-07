#include <network/tcpSocket.h>

#include <QMetaType>

static int qAbstractSocket_SocketState_registration = qRegisterMetaType<QAbstractSocket::SocketState>();
static int qAbstractSocket_SocketError_registration = qRegisterMetaType<QAbstractSocket::SocketError>();
static int qAbstractSocket_SslErrorList_registration = qRegisterMetaType<QList<QSslError>>();

tcpSocket::tcpSocket(QObject* _parent, bool is_server_socket) : QObject(_parent), socketManager(NULL)
    {
    connect(&socketManager, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));
    server_side_socket = is_server_socket;
    socketManager.moveToThread(&thread);
    }
tcpSocket::~tcpSocket()
    {
    thread.quit();
    thread.wait();
    }

bool tcpSocket::isServerSocket() const
    {
    return server_side_socket;
    }

const QSslKey& tcpSocket::privateKey() const
	{
	return sslKey;
	}
void tcpSocket::setPrivateKey(const QSslKey& key)
	{
	sslKey = key;
	}

const QSslCertificate& tcpSocket::localCertificate() const
	{
	return sslCertificate;
	}
void tcpSocket::setLocalCertificate(const QSslCertificate& certificate)
	{
	sslCertificate = certificate;
	}

void tcpSocket::create(QHostAddress _address, qint16 _port)
	{

	if (thread.isRunning() == true)
		{
		thread.quit();
		thread.wait();
		}
    QSharedPointer<QSslSocket> socket = QSharedPointer<QSslSocket>(new QSslSocket(NULL));
    if (socket.isNull() == false)
        {
        //socket.data()->moveToThread(&thread);
		socket.data()->connectToHostEncrypted(_address.toString(), _port);
		// For now disable SSL Verification - just we can get going.
		// TODO: Add in proper SSL configuration with PEM files, etc
		socket.data()->setPeerVerifyMode(QSslSocket::VerifyNone);

        socketManager.setupSocket(socket);
        connect(socket.data(), SIGNAL(disconnected()), &thread, SLOT(quit()));
		connect(socket.data(), SIGNAL(encrypted()), this, SLOT(encrypted()));
		connect(socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
		connect(socket.data(), SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));
        thread.start();
        }
	}
void tcpSocket::destroy()
	{
	if (socketManager.getSocket().isNull() == false)
		{
		socketManager.getSocket()->close();
		}
	}

void tcpSocket::onSslErrors(const QList<QSslError>& errors)
	{
	for (auto iter = errors.begin(); iter != errors.end(); ++iter)
		{
		Q_EMIT logMessage(QString("SSL ERROR: %1").arg(iter->errorString()));
		}
	}
void tcpSocket::onSocketError(QAbstractSocket::SocketError _error)
	{
	switch(_error)
		{
		case QAbstractSocket::ConnectionRefusedError:
			Q_EMIT logMessage("ERROR: Connection Refused by peer or time-out occurred");
			break;
		case QAbstractSocket::RemoteHostClosedError:
			Q_EMIT logMessage("ERROR: Remote Host closed the connection");
			break;
		case QAbstractSocket::HostNotFoundError:
			Q_EMIT logMessage("ERROR: Remote Host Not Found");
			break;
		case QAbstractSocket::SocketAccessError:
			Q_EMIT logMessage("ERROR: Socket Operation because application lacked priviledges");
			break;
		case QAbstractSocket::SocketResourceError:
			Q_EMIT logMessage("ERROR: Local system is out of resources");
			break;
		case QAbstractSocket::SocketTimeoutError:
			Q_EMIT logMessage("ERROR: Socket Timed out");
			break;
		case QAbstractSocket::DatagramTooLargeError:
			Q_EMIT logMessage("ERROR: Datagram was larger than the system allowed");
			break;
		case QAbstractSocket::NetworkError:
			Q_EMIT logMessage("ERROR: Network Error occurred. Cable plugged in?");
			break;
		case QAbstractSocket::AddressInUseError:
			Q_EMIT logMessage("ERROR: Address Already In Use");
			break;
		case QAbstractSocket::SocketAddressNotAvailableError:
			Q_EMIT logMessage("ERROR: Specified Address does not belong to the host");
			break;
		case QAbstractSocket::UnsupportedSocketOperationError:
			Q_EMIT logMessage("ERROR: Socket operation not supported by host system");
			break;
		case QAbstractSocket::ProxyAuthenticationRequiredError:
			Q_EMIT logMessage("ERROR: Intermediate Proxy requires authentication");
			break;
		case QAbstractSocket::SslHandshakeFailedError:
			Q_EMIT logMessage("ERROR: SSL/TLS handshake failed.");
			break;
		case QAbstractSocket::UnfinishedSocketOperationError:
			Q_EMIT logMessage("ERROR: Last operation in progress has not yet completed");
			break;
		case QAbstractSocket::ProxyConnectionRefusedError:
			Q_EMIT logMessage("ERROR: Could not contact the intermediate Proxy Server");
			break;
		case QAbstractSocket::ProxyConnectionClosedError:
			Q_EMIT logMessage("ERROR: Proxy connection was unexpectedly closed");
			break;
		case QAbstractSocket::ProxyConnectionTimeoutError:
			Q_EMIT logMessage("ERROR: Proxy connection timed-out");
			break;
		case QAbstractSocket::ProxyNotFoundError:
			Q_EMIT logMessage("ERROR: Proxy address was not found");
			break;
		case QAbstractSocket::ProxyProtocolError:
			Q_EMIT logMessage("ERROR: Message from intermediate Proxy Server could not be understood");
			break;
		case QAbstractSocket::OperationError:
			Q_EMIT logMessage("ERROR: Operation attempted while not in a valid state for that operation");
			break;
		case QAbstractSocket::SslInternalError:
			Q_EMIT logMessage("ERROR: SSL library reported an internal error. Check SSL library installation.");
			break;
		case QAbstractSocket::SslInvalidUserDataError:
			Q_EMIT logMessage("ERROR: Invalid data provided to SSL library that resulted in an error. Check configuration.");
			break;
		case QAbstractSocket::TemporaryError:
			Q_EMIT logMessage("ERROR: Temporary Error Occurred. Please try again");
			break;
		case QAbstractSocket::UnknownSocketError:
			Q_EMIT logMessage("ERROR: Unknown Socket Error occurred");
			break;
		default:
			Q_EMIT logMessage(QString("ERROR: Unknown Error (%1) occurred").arg(_error));
			break;
		};
	}
void tcpSocket::setSocketDescriptor(qintptr socketDescriptor)
    {
    if (thread.isRunning() == true)
        {
        thread.quit();
        thread.wait();
        }
    QSharedPointer<QSslSocket> socket = QSharedPointer<QSslSocket>(new QSslSocket(NULL));
    if (socket.isNull() == false)
        {
        socket.data()->setSocketDescriptor(socketDescriptor);
		// For now disable SSL Verification - just we can get going.
        socket.data()->moveToThread(&thread);

        socketManager.setupSocket(socket);
        connect(socket.data(), SIGNAL(disconnected()), &thread, SLOT(quit()));
		connect(socket.data(), SIGNAL(encrypted()), this, SLOT(encrypted()));
		connect(socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
		connect(socket.data(), SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));

		socket.data()->setPrivateKey(sslKey);
		socket.data()->setLocalCertificate(sslCertificate);
        thread.start();
        }
    }
void tcpSocket::encrypted()
	{
	Q_EMIT logMessage("Connection Encrypted");
	}
QSharedPointer<QSslSocket> tcpSocket::getSocket() const
    {
    return socketManager.getSocket();
    }
