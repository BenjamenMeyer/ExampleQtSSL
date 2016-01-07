#include <session/clientSessionManager.h>

clientSessionManager::clientSessionManager(QObject* _parent) : QObject(_parent)
	{
	}
clientSessionManager::~clientSessionManager()
	{
	for (auto iter = sessionList.begin(); iter != sessionList.end(); ++iter)
		{
		iter->data()->thread.quit();
		}
	for (auto iter = sessionList.begin(); iter != sessionList.end(); ++iter)
		{
		iter->data()->thread.wait();
		}
	}

ptrClientSession clientSessionManager::newClientSession()
	{
	ptrClientSessionInfo newSession = ptrClientSessionInfo(new struct clientSessionInfo);
	ptrClientSession returnValue;
	if (newSession.isNull() == false)
		{
		newSession.data()->session = ptrClientSession(new clientSession);
		if (newSession.data()->session.isNull() == false)
			{
			newSession.data()->session->moveToThread(&(newSession.data()->thread));
			newSession.data()->thread.start();
			returnValue = newSession.data()->session;

			sessionList.push_back(newSession);
			}
		}
	return returnValue;
	}
void clientSessionManager::linkToMasterSession(QString _session_id, QString _sub_session_id)
	{
	// first look up the sub-session-id
	clientSessionList::iterator master = sessionList.end();
	clientSessionList::iterator worker = sessionList.end();
	for (auto client = sessionList.begin(); client != sessionList.end(); ++client)
		{
		if (client->data()->session.isNull() == false)
			{
			if (client->data()->session.data()->getSessionId() == _session_id)
				{
				client->data()->dataSessions.append(_sub_session_id);
				master = client;
				}
			else if (client->data()->session.data()->getSubSessionId() == _sub_session_id)
				{
				worker = client;
				}
			}
		}

	if (master != sessionList.end() && worker != sessionList.end())
		{
		// TODO: Connect up signals between the master session and the worker session

		// 
		}
	else
		{
		if (master == sessionList.end())
			{
			Q_EMIT logMessage(tr("{0}/{1} - unable to locate master").arg(_session_id).arg(_sub_session_id));
			}
		if (worker == sessionList.end())
			{
			Q_EMIT logMessage(tr("{0}/{1} - unable to locate worker").arg(_session_id).arg(_sub_session_id));
			}
		}
	}
