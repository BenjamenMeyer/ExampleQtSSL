#ifndef QT_SSL_EXAMPLE_CLIENT_SESSION_MANAGER_H__
#define QT_SSL_EXAMPLE_CLIENT_SESSION_MANAGER_H__

#include <QObject>
#include <QThread>
#include <QList>
#include <QString>
#include <QStringList>

#include <session/clientSession.h>

class clientSessionManager : public QObject
	{
	Q_OBJECT
	public:
		clientSessionManager(QObject* _parent=NULL);
		virtual ~clientSessionManager();

		ptrClientSession newClientSession();

	Q_SIGNALS:
		void logMessage(QString _message);

	protected:
		struct clientSessionInfo
			{
			QThread thread;
			ptrClientSession session;
			QString controlSession;
			QStringList dataSessions;
			};
		typedef QSharedPointer<struct clientSessionInfo> ptrClientSessionInfo;
		typedef QList<ptrClientSessionInfo> clientSessionList;

		clientSessionList sessionList;

	protected Q_SLOTS:

		void linkToMasterSession(QString _session_id, QString _sub_session_id);
	};

#endif //QT_SSL_EXAMPLE_CLIENT_SESSION_MANAGER_H__
