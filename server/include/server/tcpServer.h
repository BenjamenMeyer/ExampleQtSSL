#ifndef QT_SSL_EXAMPLE_TCP_SERVER_H__
#define QT_SSL_EXAMPLE_TCP_SERVER_H__

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslCertificate>
#include <QPointer>

#include <network/tcpSocket.h>

namespace exampleQtSsl
    {
    namespace server
        {
        namespace network
            {

            class tcpServer : public QTcpServer
                {
                Q_OBJECT
                public:
                    tcpServer(QObject* _parent=NULL);
                    virtual ~tcpServer();

                    exampleQtSsl::common::network::ptrTcpSocket getManagerForConnection(QTcpSocket* _socket);

					void setSslKey(QSslKey& _key);
					void setSslCertificate(QSslCertificate& _certificate);

                public Q_SLOTS:
                Q_SIGNALS:
                    // new client connections that live in their own threads
                    // the parameter should only be used to establish signals/slot
                    // connections
                    void newConnection(QSharedPointer<QSslSocket> _new_ssl_socket);
                    void logMessage(QString _message);

                protected:
                    virtual void incomingConnection(qintptr socketDescriptor);

                protected Q_SLOTS:
                private:
                    exampleQtSsl::common::network::listTcpSocket clients;
					QSslKey sslKey;
					QSslCertificate sslCertificate;

                private Q_SLOTS:
                };

            typedef QPointer<tcpServer> ptrTcpServer;

            }
        }
    }

#endif //QT_SSL_EXAMPLE_TCP_SERVER_H__
