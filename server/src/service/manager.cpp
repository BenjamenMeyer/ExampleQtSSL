#include <service/manager.h>

using namespace exampleQtSsl::server::service;

manager::manager(QObject* _parent) : QObject(_parent), address(QHostAddress::Any), port(9000), theTcpServer(this)
    {
    connect(&theTcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(&theTcpServer, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));
    }
manager::~manager()
    {
    }

void manager::setServerConfig(QHostAddress _address, qint16 _port)
    {
    if (theTcpServer.isListening() == false)
        {
        Q_EMIT logMessage(tr("DEBUG: Server is NOT active. Updating address from [%1]:%2 to [%3]:%4").arg(address.toString()).arg(port).arg(_address.toString()).arg(_port));
        address = _address;
        port = _port;
        }
    else
        {
        Q_EMIT logMessage(tr("DEBUG: Server is active. Unable to update address from [%1]:%2 to [%3]:%4").arg(address.toString()).arg(port).arg(_address.toString()).arg(_port));
        }
    }
void manager::setMaxPending(int _connections)
    {
    theTcpServer.setMaxPendingConnections(_connections);
    }
void manager::setSslKey(QSslKey& _key)
	{
	theTcpServer.setSslKey(_key);
	}
void manager::setSslCertificate(QSslCertificate& _certificate)
	{
	theTcpServer.setSslCertificate(_certificate);
	}

void manager::start_service()
    {
    if (theTcpServer.isListening() == false)
        {
        Q_EMIT logMessage(tr("Starting Service"));
        theTcpServer.listen(address, port);
        }
    else
        {
        Q_EMIT logMessage(tr("Server already listening"));
        }
    }
void manager::stop_service()
    {
    if (theTcpServer.isListening() == true)
        {
        Q_EMIT logMessage(tr("Stopping Service"));
        theTcpServer.close();
        }
    else
        {
        Q_EMIT logMessage(tr("Server already stopped"));
        }
    }

void manager::acceptError(QAbstractSocket::SocketError socketError)
    {
    switch (socketError)
        {
        default:
            Q_EMIT logMessage(tr("Unknown connection acceptance error - %1").arg(socketError));
            break;
        };
    }
void manager::newConnection()
    {
    QTcpSocket* newTcpConnection = theTcpServer.nextPendingConnection();
    if (newTcpConnection != NULL)
        {
        exampleQtSsl::common::network::ptrTcpSocket newClient = theTcpServer.getManagerForConnection(newTcpConnection);
        }
    }

