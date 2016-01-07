#include <session/clientSession.h>

#include <QJsonDocument>
#include <QUuid>

clientSession::clientSession(QObject* _parent) : QObject(_parent)
	{
	}
clientSession::~clientSession()
	{
	}

/* static */ QUuid clientSession::createSessionId()
	{
	return QUuid::createUuid();
	}

QString clientSession::getSessionId() const
	{
	QString returnValue;
	if (session_id.isNull() == false)
		{
		returnValue = session_id.toString();
		}
	return returnValue;
	}
QString clientSession::getSubSessionId() const
	{
	QString returnValue;
	if (sub_session_id.isNull() == false)
		{
		returnValue = sub_session_id.toString();
		}
	return returnValue;
	}

void clientSession::receivedMessage(QByteArray _data)
	{
	}

void clientSession::receiveJson(QByteArray _jsonDocument)
	{
	QJsonDocument doc = QJsonDocument::fromBinaryData(_jsonDocument);
	if (doc.isNull() == false)
		{
		processMessage(doc.object());
		}
	}

void clientSession::loadSession(QJsonObject _message)
	{
	QJsonObject session;
	bool create_session = true;
	bool create_sub_session = true;
	if (_message.contains("session") == true)
		{
		session = _message["session"].toObject();
		if (session.contains("id") == true)
			{
			session_id = session["id"];
			}
		if (session.contains("sub-session") == true)
			{
			sub_session_id = session["sub-session"];
			}

		if (session_id.toString() != QString("new"))
			{
			QUuid validate = QUuid(session_id.toString());
			create_session = (validate.isNull() == true);
			}
		if (sub_session_id.toString() != QString("new"))
			{
			QUuid validate = QUuid(sub_session_id.toString());
			create_sub_session = (validate.isNull() == true);
			}
		}
	
	if (create_session == true)
		{
		session_id = clientSession::createSessionId().toString();
		}
	if (create_sub_session == true)
		{
		sub_session_id = clientSession::createSessionId().toString();
		}
	}
void clientSession::processMessage(QJsonObject _message)
	{
	QJsonObject response;
	loadSession(_message);

	response.insert("session", session_id);
	}

void clientSession::sendBinaryMessage(QByteArray _key, QByteArray _data)
	{
	}
