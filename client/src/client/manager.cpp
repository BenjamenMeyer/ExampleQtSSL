#include <client/manager.h>

using namespace exampleQtSsl::client::network;

manager::manager(QObject* _parent) : QObject(_parent), control(NULL)
	{
	connect(&control, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));
	}
manager::~manager()
	{
	}

void manager::start_control()
	{
	control.create(QHostAddress("127.0.0.1"), 9000);
	}
void manager::stop_control()
	{
	control.destroy();
	}

void manager::add_data()
	{
	}
void manager::remove_data()
	{
	}

