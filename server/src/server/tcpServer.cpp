#include <server/tcpServer.h>

using namespace exampleQtSsl::server::network;


tcpServer::tcpServer(QObject* _parent) : QTcpServer(_parent)
    {
    }
tcpServer::~tcpServer()
    {
    }

void tcpServer::setSslKey(QSslKey& _key)
	{
	sslKey = _key;
	}
void tcpServer::setSslCertificate(QSslCertificate& _certificate)
	{
	sslCertificate = _certificate;
	}

void tcpServer::incomingConnection(qintptr socketDescriptor)
    {
    exampleQtSsl::common::network::ptrTcpSocket socketThread = QSharedPointer<exampleQtSsl::common::network::tcpSocket>(new exampleQtSsl::common::network::tcpSocket(NULL));
    if (socketThread.isNull() == false)
        {
        connect(socketThread.data(), SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));
		socketThread->setPrivateKey(sslKey);
		socketThread->setLocalCertificate(sslCertificate);
        socketThread->setSocketDescriptor(socketDescriptor);
        clients.prepend(socketThread);

        Q_EMIT logMessage(tr("DEBUG: Received connection from [%1]:%2").arg(socketThread.data()->getSocket().data()->peerAddress().toString()).arg(socketThread.data()->getSocket().data()->peerPort()));

        addPendingConnection(socketThread.data()->getSocket().data());

        Q_EMIT newConnection(socketThread->getSocket());
        }
    }

exampleQtSsl::common::network::ptrTcpSocket tcpServer::getManagerForConnection(QTcpSocket* _socket)
    {
    Q_EMIT logMessage(tr("Looking up Connection Manager for new tcp connection..."));
    QSslSocket* realSocket = (QSslSocket*) _socket;
    for (auto iter = clients.begin(); iter != clients.end(); ++iter)
        {
        if (iter->isNull() == false)
            {
            if (iter->data()->getSocket() == realSocket)
                {
                Q_EMIT logMessage(tr("Found existing Connection Manager..."));
                return (*iter);
                }
            }
        }
	Q_EMIT logMessage(tr("No existing Connection Manager could be found"));
    return exampleQtSsl::common::network::ptrTcpSocket();
    }
