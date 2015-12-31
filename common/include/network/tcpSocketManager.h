#ifndef QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_MANAGER_H__
#define QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_MANAGER_H__

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QtNetwork/QSslSocket>

namespace exampleQtSsl
    {
    namespace common
        {
        namespace network
            {

            enum messageTypes
                {
                jsonMessageType = 1,
                binaryMessageType = 2
                };

            struct messageHeader
                {
                uint32_t length;
                uint32_t type;
                };

            struct jsonMessage
                {
                struct messageHeader header;
                uint8_t* data[];
                };

            struct binaryMessage
                {
                struct messageHeader header;
                uint8_t* data[];
                };

            class tcpSocketManager : public QObject
                {
                Q_OBJECT
                public:
                    tcpSocketManager(QObject* _parent=NULL);
                    virtual ~tcpSocketManager();

                    void setupSocket(QSharedPointer<QSslSocket> _socket);
                    QSharedPointer<QSslSocket> getSocket() const;

                public Q_SLOTS:
                    void sendMessage(QByteArray _data);
                    void sendJson(QByteArray _jsonDocument);

                Q_SIGNALS:
                    void receivedMessage(QByteArray _data);
                    void receivedJson(QByteArray _jsonDocument);
                    void logMessage(QString _message);

                protected:
                    // socket
                    QSharedPointer<QSslSocket> connection;

                    // receive data
                    void sendData(QByteArray _messageHeader, QByteArray _data);

                protected Q_SLOTS:
                    void dataAvailable();
					void socketStateChanged(QAbstractSocket::SocketState _state);

                private:
                private Q_SLOTS:
                };

            }
        }
    }


#endif //QT_SSL_EXAMPLE_COMMON_TCP_SOCKET_MANAGER_H__
