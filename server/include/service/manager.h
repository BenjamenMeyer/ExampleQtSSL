#ifndef QT_SSL_EXAMPLE_SERVICE_MANAGER_H__
#define QT_SSL_EXAMPLE_SERVICE_MANAGER_H__

#include <QObject>
#include <QString>
#include <QPointer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslCertificate>

#include <server/tcpServer.h>

namespace exampleQtSsl
    {
    namespace server
        {
        namespace service
            {

            class manager: public QObject
                {
                Q_OBJECT
                public:
                    manager(QObject* _parent=NULL);
                    virtual ~manager();

                public Q_SLOTS:
                    void start_service();
                    void stop_service();

                    void setServerConfig(QHostAddress _address, qint16 _port);
                    void setMaxPending(int _connections);
					void setSslKey(QSslKey& _key);
					void setSslCertificate(QSslCertificate& _certificate);

                Q_SIGNALS:
                    void logMessage(QString _message);

                protected:
                    QHostAddress address;
                    qint16 port;

                    exampleQtSsl::server::network::tcpServer theTcpServer;

                protected Q_SLOTS:
                    void newConnection();
                    void acceptError(QAbstractSocket::SocketError socketError);
                };
            typedef QPointer<manager> ptrManager;

            }
        }
    }

#endif //QT_SSL_EXAMPLE_SERVICE_MANAGER_H__
