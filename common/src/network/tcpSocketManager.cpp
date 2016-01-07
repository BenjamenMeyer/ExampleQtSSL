#include <network/tcpSocketManager.h>

tcpSocketManager::tcpSocketManager(QObject* _parent) : QObject(_parent)
    {
    }
tcpSocketManager::~tcpSocketManager()
    {
    if (connection.isNull() == false)
        {
        connection.data()->close();
        }
    }

void tcpSocketManager::setupSocket(QSharedPointer<QSslSocket> _socket)
    {
    if (_socket.isNull() == false)
        {
        connection = _socket;
        connect(connection.data(), SIGNAL(readyRead()), this, SLOT(dataAvailable()));
		connect(connection.data(), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        }
    }
QSharedPointer<QSslSocket> tcpSocketManager::getSocket() const
    {
    return connection;
    }

void tcpSocketManager::socketStateChanged(QAbstractSocket::SocketState _state)
	{
	switch (_state)
		{
		case QAbstractSocket::UnconnectedState:
			Q_EMIT logMessage("Socket Disconnected");
			break;

		case QAbstractSocket::HostLookupState:
			Q_EMIT logMessage("Looking up host...");
			break;

		case QAbstractSocket::ConnectingState:
			Q_EMIT logMessage("Connecting socket...");
			break;

		case QAbstractSocket::ConnectedState:
			Q_EMIT logMessage("Socket Connected");
			break;

		case QAbstractSocket::BoundState:
			Q_EMIT logMessage("Socket Bound (Server)");
			break;

		case QAbstractSocket::ClosingState:
			Q_EMIT logMessage("Closing Socket");
			break;

		case QAbstractSocket::ListeningState:
			Q_EMIT logMessage("Socket Listening for connections...");
			break;

		default:
			Q_EMIT logMessage(QString("Unknown Socket State: %1").arg(_state));
			break;
		};
	}


void tcpSocketManager::sendMessage(QByteArray _data)
    {
    /*
        add the header, write to the socket
     */
    struct messageHeader msgHeader;
    msgHeader.length = sizeof(struct messageHeader) + _data.size();
    msgHeader.type = binaryMessageType;
    QByteArray headerData((const char*)&msgHeader, sizeof(struct messageHeader));
    sendData(headerData, _data);
    }
void tcpSocketManager::sendJson(QByteArray _jsonDocument)
    {
    /*
        add the header, write to the socket
     */
    struct messageHeader msgHeader;
    msgHeader.length = sizeof(struct messageHeader) + _jsonDocument.size();
    msgHeader.type = jsonMessageType;
    QByteArray headerData((const char*)&msgHeader, sizeof(struct messageHeader));
    sendData(headerData, _jsonDocument);
    }
void tcpSocketManager::sendData(QByteArray _messageHeader, QByteArray _data)
    {
    // doesn't matter what we do if there's nothing to write to
    if (connection.isNull() == false)
        {
        qint64 header_write = connection.data()->write(_messageHeader);

        if (header_write == _messageHeader.length())
            {
            qint64 data_write = connection.data()->write(_data);
            }
        else
            {
            Q_EMIT logMessage(tr("DEBUG: Failed to write message header. Only wrote %1 bytes").arg(header_write));
            }
        }
    }

void tcpSocketManager::dataAvailable()
    {
    /*
        Peek to see if all the data is available
        If it is, read it, remove the header, and send it up the chain using the associated signal
     */
    if (connection.isNull() == false)
        {
        const size_t header_size = sizeof(messageHeader);

        if (connection.data()->bytesAvailable() > header_size)
            {
            struct messageHeader header;
            memset(&header, 0, header_size);

            qint64 header_peek = connection.data()->peek((char*)&header, header_size);
            if (header_peek == header_size)
                {
                if (connection.data()->bytesAvailable() >= header.length)
                    {
                    struct messageHeader dummyHeader;
                    memset(&header, 0, header_size);
                    qint64 header_read = connection.data()->read((char*)&header, header_size);

                    if (header_read == header_size)
                        {
                        int message_size = header.length - header_size;
                        if (message_size > 0)
                            {
                            QByteArray message(message_size, 0);
                            qint64 message_read = connection.data()->read(message.data(), message_size);

                            switch (header.type)
                                {
                                case jsonMessageType:
									Q_EMIT logMessage(tr("DEBUG: Received JSON Message"));
                                    Q_EMIT receivedMessage(message);
                                    break;

                                case binaryMessageType:
									Q_EMIT logMessage(tr("DEBUG: Received Binary Message"));
                                    Q_EMIT receivedJson(message);
                                    break;

                                default:
                                    Q_EMIT logMessage(tr("DEBUG: received message with unknown message type (%1)").arg(header.type));
                                };
                            }
                        else
                            {
                            Q_EMIT logMessage(tr("DEBUG: received message with invalid size of %1 bytes.").arg(message_size));
                            }
                        }
                    else
                        {
                        Q_EMIT logMessage(tr("DEBUG: Failed to read message header. Only read %1 bytes").arg(header_read));
                        }
                    }
                }
            }
        }
    }
