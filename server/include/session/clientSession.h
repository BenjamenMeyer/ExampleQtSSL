#ifndef QT_SSL_EXAMPLE_CLIENT_SESSION_H__
#define QT_SSL_EXAMPLE_CLIENT_SESSION_H__

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include <QString>
#include <QJsonObject>
#include <QJsonValue>

namespace exampleQtSsl
	{
	namespace server
		{
		namespace session
			{

			class clientSession: public QObject
				{
				Q_OBJECT
				public:
					clientSession(QObject* _parent=NULL);
					virtual ~clientSession();

					QString getSessionId() const;
					QString getSubSessionId() const;

				public Q_SLOTS:
                    void receivedMessage(QByteArray _data);
                    void receiveJson(QByteArray _jsonDocument);

					// to client
					void sendBinaryMessage(QByteArray _key, QByteArray _data);

				Q_SIGNALS:
                    void sendMessage(QByteArray _data);
                    void sendJson(QByteArray _jsonDocument);
                    void logMessage(QString _message);

					// from client
					void receiveBinaryMessage(QByteArray _key, QByteArray _data);

				protected:
					QJsonValue session_id;
					QJsonValue sub_session_id;

					static QUuid createSessionId();

					void processMessage(QJsonObject _message);
					void loadSession(QJsonObject _message);

				protected Q_SLOTS:
				private:
				private Q_SLOTS:
				};

			typedef QSharedPointer<clientSession> ptrClientSession;
			}
		}
	}

#endif //QT_SSL_EXAMPLE_CLIENT_SESSION_H__
